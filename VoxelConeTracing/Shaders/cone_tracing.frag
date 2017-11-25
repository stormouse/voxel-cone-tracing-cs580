#version 430 core

#define VOXEL_SIZE (1/64.0)
#define MIPMAP_CAP 5.4f
#define HSQRT2  0.707106    //square root of 2 divided by 2
#define SQRT2   1.414213    //square root of 2
#define SPECULAR_FACTOR 4.0f
#define SPECULAR_POWER 65.0f
#define DIRECT_LIGHT_INTENSITY 0.96f

in vec3 worldPosFrag;
in vec3 normalFrag;

// camera info
uniform vec3    cameraPosition; 

//light info
uniform vec3 	lightPosition;
uniform vec3 	lightColor;
uniform float   lightIntensity;

//material info
uniform vec3    diffuseColor;
uniform	float   diffuseReflectivity;
uniform	vec3    specularColor;
uniform	float   specularDiffusion; // "Reflective and refractive" specular diffusion. 
uniform	float   specularReflectivity;
uniform	float   emissivity; // Emissive materials uses diffuse color as emissive color.
uniform	float   refractiveIndex;
uniform	float   transparency;
//texture info
uniform sampler3D texture3D;
//voxel info
uniform int voxelDimension;

float voxel_size = 1.0f/voxelDimension;
float max_dis = distance(vec3(abs(worldPosFrag)), vec3(-1));
//normalFrag = normalize(normalFrag);
//vec3 normal = normalize(normalFrag); 

out vec4 color;

float attenuate(float dist){ 
    float dist_factor = 1.1f;
    float constant = 1.0f;
    float linear = 0.0f;
    float quadratic = 1.0f;


    dist *= dist_factor;
    return 1.0f / (constant + linear * dist + quadratic * dist * dist); 
    }

//test with random orthogonal vector 
vec3 orthogonal(vec3 a){
    a = normalize(a);
    vec3 b = vec3(0.81818f, 0.09090f, 0.09090f); 
    if(abs(dot(a, b)) > 0.99999f)
        return cross(a, vec3(0.0f, 1.0f, 0.0f));
    else
        return cross(a, b);
}

vec3 scaleAndBias(vec3 a) { 
    return 0.5f * a + vec3(0.5f); 
}

bool isInsideCube(const vec3 p) { 
    return abs(p.x) < 1 && abs(p.y) < 1 && abs(p.z) < 1; 
}

//calculate shadow by tracing shadow cone
float calShadow(vec3 origin, vec3 direction, float distance){
	origin += normalFrag * 0.05f; 
	float accColor = 0.0f;
	float dist = 3 * voxel_size;
	
	const float stopSign = distance - 16 * voxel_size;

	while(dist < stopSign && accColor < 1){	
		vec3 v = origin + dist * direction;
		if(!isInsideCube(v)) break;
		v = scaleAndBias(v);
		float l = pow(dist, 2); // Experimenting with inverse square falloff for shadows.
		float shadow1 = 0.062 * textureLod(texture3D, v, 1 + 0.75 * l).a;
		float shadow2 = 0.135 * textureLod(texture3D, v, 4.5 * l).a;
		float shadow = shadow1 + shadow2;
		accColor += (1 - accColor) * shadow;
		dist += 0.9 * voxel_size * (1 + 0.05 * l);
	}
	return 1 - pow(smoothstep(0, 1, accColor * 1.4), 1.0 / 1.4);
}	


//diffuse voxel cone tracing
vec3 diffuseVoxelConeTracing(const vec3 origin, vec3 direction){
    direction = normalize(direction);
    const float cone_spread = 0.325;
    vec4 accColor = vec4(0.0f);

    //lower value should be tried
    float controlDist = 0.1953125;

    while(controlDist < SQRT2 && accColor.a < 1){
        vec3 v = origin + controlDist*direction;
        v = scaleAndBias(origin + controlDist * direction);
        float lI = 1 + cone_spread * controlDist / voxel_size;
        float level = log2(lI);
        //float ll = (level + 1) * (level + 1);
        float ll = pow((level +1 ), 2.0f);
        vec4 voxel = textureLod(texture3D, v, min(MIPMAP_CAP, level));
        accColor += 0.075 * ll * voxel * pow(1 - voxel.a, 2);
        controlDist += ll * voxel_size * 2;
    }
    return pow(accColor.rgb * 2.0, vec3(1.5));
}

vec3 specularVoxelConeTracing(vec3 origin, vec3 direction){
    direction = normalize(direction);
    //??
    const float offset = 8 * voxel_size;
    const float step = voxel_size;


    origin += offset*normalFrag;
    vec4 accColor = vec4(0.0f);
    float dist = offset;
    

    while(dist < max_dis && accColor.a < 1){
        vec3 v = origin + dist * direction;
		if(!isInsideCube(v)) break;
		v = scaleAndBias(v); 
		
		float level = 0.1 * specularDiffusion * log2(1 + dist / voxel_size);
		vec4 voxel = textureLod(texture3D, v, min(level, MIPMAP_CAP));
		float f = 1 - accColor.a;
		accColor.rgb += 0.25 * (1 + specularDiffusion) * voxel.rgb * voxel.a * f;
		accColor.a += 0.25 * voxel.a * f;
		dist += step * (1.0f + 0.125f * level);
    }
    return 1.0 * pow(specularDiffusion + 1, 0.8) * accColor.rgb;
}

vec3 indirectDiffuseLight(){
    const float angle_mix = 0.5f;
    const float weight[3] = {1.0, 1.0, 1.0};

    const vec3 base1 = normalize(orthogonal(normalFrag));
	const vec3 base2 = normalize(cross(base1, normalFrag));

    const vec3 cornerB = 0.5f * (base1 + base2);
	const vec3 cornerB2 = 0.5f * (base1 - base2);

    const vec3 n_offset = normalFrag * (1 + 4 * HSQRT2) * voxel_size;
	const vec3 origin = worldPosFrag + n_offset;

    vec3 accColor = vec3(0.0f);
    const float cone_offset = -0.01;

    //trace front cone
    accColor += weight[0] * diffuseVoxelConeTracing(origin + cone_offset * normalFrag, normalFrag);

    //trace side cone: 4side?????????
    const vec3 s1 = mix(normalFrag, base1, angle_mix);
	const vec3 s2 = mix(normalFrag, -base1, angle_mix);
	const vec3 s3 = mix(normalFrag, base2, angle_mix);
	const vec3 s4 = mix(normalFrag, -base2, angle_mix);

    accColor += weight[1] * diffuseVoxelConeTracing(origin + cone_offset * base1, s1);
    accColor += weight[1] * diffuseVoxelConeTracing(origin + cone_offset * base1, s2);
    accColor += weight[1] * diffuseVoxelConeTracing(origin + cone_offset * base2, s3);
    accColor += weight[1] * diffuseVoxelConeTracing(origin + cone_offset * base2, s4);

    const vec3 c1 = mix(normalFrag, cornerB, angle_mix);
    const vec3 c2 = mix(normalFrag, -cornerB, angle_mix);
    const vec3 c3 = mix(normalFrag, cornerB2, angle_mix);
    const vec3 c4 = mix(normalFrag, -cornerB2, angle_mix);

    accColor += weight[2] * diffuseVoxelConeTracing(origin + cone_offset * cornerB, c1);
    accColor += weight[2] * diffuseVoxelConeTracing(origin + cone_offset * cornerB, c2);
    accColor += weight[2] * diffuseVoxelConeTracing(origin + cone_offset * cornerB2, c3);
    accColor += weight[2] * diffuseVoxelConeTracing(origin + cone_offset * cornerB2, c4);
    //meaning of vec3(0.001f)??????
    return 0.52 * diffuseReflectivity * accColor * (diffuseColor + vec3(0.001f));
}

vec3 calIndirectSpecLight(vec3 viewDirect){
	const vec3 reflection = normalize(reflect(viewDirect, normalFrag));
	return specularReflectivity * specularColor * specularVoxelConeTracing(worldPosFrag, reflection);
}

//the material is transparency
vec3 indirectRefractiveLight(vec3 viewDirect){
	const vec3 refraction = refract(viewDirect, normalFrag, 1.0 / refractiveIndex);
	const vec3 cmix = mix(specularColor, 0.5 * (specularColor + vec3(1)), transparency);
	return cmix * specularVoxelConeTracing(worldPosFrag, refraction);
}

//calculate the diffuse and specular part for the given pointlight
//vec3 calDirectLight(vec3 lightPosition, vec3 lightColor, vec3 viewDirect){
vec3 calDirectLight(vec3 viewDirect){
    vec3 lightDirect = lightPosition - worldPosFrag;
    const float lightDistance = length(lightDirect);
    //lightDirect = normalize(lightDirect);
    lightDirect = lightDirect/lightDistance;
    const float lightAngle = dot(lightDirect, normalFrag);
    
    //diffuse light
    float diffuseAngle = max(0.0f, lightAngle);

    //specular light
    //perfect reflection
    const vec3 viewReflectDirection = normalize(reflect(viewDirect, normalFrag));
    float specAngle = max(dot(lightDirect, viewReflectDirection), 0.0f);    

    float refractiveAngle = 0;
	if(transparency > 0.01){
		vec3 refraction = refract(viewDirect, normalFrag, 1.0 / refractiveIndex);
		refractiveAngle = max(0, transparency * dot(refraction, lightDirect));
	}

    //shadow
    float shadowBlend = 1.0f;
    if(diffuseAngle * (1.0f - transparency) > 0){
        shadowBlend = calShadow(worldPosFrag, lightDirect, lightDistance);
    }

    //add all
    diffuseAngle = min(shadowBlend, diffuseAngle);
    //diffuseAngle = min(shadowBlend, 1);
    //specAngle = min(shadowBlend, max(specAngle, refractiveAngle));
    specAngle = min(shadowBlend,specAngle);
    const float diffuseFactor = 1.0f / (1.0f + 0.25f * specularDiffusion); // Diffusion factor.
    const float specular = SPECULAR_FACTOR * pow(specAngle, diffuseFactor * SPECULAR_POWER);
	const float diffuse = diffuseAngle * (1.0f - transparency);

	const vec3 diff = diffuseReflectivity * diffuseColor * diffuse;
	const vec3 spec = specularReflectivity * specularColor * specular;
	const vec3 total = lightColor * (diff + spec);

	return attenuate(lightDistance) * total;
}

vec3 directLight(vec3 viewDirect){
	vec3 direct = vec3(0.0f);
	//const uint maxLights = min(numberOfLights, 1);
	//for(uint i = 0; i < maxLights; ++i) 
    direct += calDirectLight(viewDirect);
	direct *= DIRECT_LIGHT_INTENSITY;
    //direct *= lightIntensity;
	return direct;
}

void main(){
    //set the initial value
    color = vec4(vec3(0.0f), 1.0);

    //calculate the direction from view to fragment
    const vec3 viewDirect = normalize(worldPosFrag - cameraPosition);

    // Indirect diffuse light.
	if(diffuseReflectivity * (1.0f - transparency) > 0.01f) 
		color.rgb += indirectDiffuseLight();

	// Indirect specular light (glossy reflections).
	if(specularReflectivity * (1.0f - transparency) > 0.01f) 
		color.rgb += calIndirectSpecLight(viewDirect);

	// Emissivity.
	color.rgb += emissivity * diffuseColor;

	// Transparency
	if(transparency > 0.01f)
		color.rgb = mix(color.rgb, indirectRefractiveLight(viewDirect), transparency);

	// Direct light.
	color.rgb += directLight(viewDirect);

#if (GAMMA_CORRECTION == 1)
	color.rgb = pow(color.rgb, vec3(1.0 / 2.2));
#endif
}