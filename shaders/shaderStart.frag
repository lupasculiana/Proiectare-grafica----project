#version 410 core

in vec3 fNormal;
in vec4 fPosEye;
in vec2 fTexCoords;
in vec4 fragPosLightSpace;
in vec4 lightPosEye;

out vec4 fColor;

//lighting
uniform	vec3 lightDir;
uniform	vec3 lightColor;

//texture
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D shadowMap;
uniform samplerCube skybox;

uniform bool pointLight;
uniform int withFog;

vec3 ambient;
float ambientStrength = 0.2f;
vec3 diffuse;
vec3 specular;
vec3 colorFromSkybox;
float specularStrength = 0.5f;
float shininess = 32.0f;

float constant = 1.0f;
float linear = 0.7f;
float quadratic = 0.8f;

void computeLightComponents()
{		
	vec3 cameraPosEye = vec3(0.0f);//in eye coordinates, the viewer is situated at the origin
	
	//transform normal
	vec3 normalEye = normalize(fNormal);	
	
	//compute light direction
	vec3 lightDirN = normalize(lightDir);
	
	//compute view direction 
	vec3 viewDirN = normalize(cameraPosEye - fPosEye.xyz);
		
	//compute ambient light
	ambient = ambientStrength * lightColor;
	
	//compute diffuse light
	diffuse = max(dot(normalEye, lightDirN), 0.0f) * lightColor;
	
	//compute specular light
	vec3 reflection = reflect(-lightDirN, normalEye);
	float specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
	specular = specularStrength * specCoeff * lightColor;

}

float computeShadow() {
	vec3 normalizedCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	if(normalizedCoords.z > 1.0f)
        return 0.0f;
	normalizedCoords = normalizedCoords * 0.5 + 0.5;
	float closestDepth = texture(shadowMap, normalizedCoords.xy).r;
	float currentDepth = normalizedCoords.z;


	float bias=0.005f;
	float shadow =currentDepth-bias>closestDepth ? 1.0f : 0.0f;

	
	return shadow;
}

float computeFog()
{
	float fogDensity = 0.08f;
	float fragmentDistance = length(fPosEye);
	float fogFactor = exp(-pow(fragmentDistance*fogDensity, 2));
	
	return clamp(fogFactor, 0.0f, 1.0f);
}

void pointLightFunc()
{		
	//compute distance to light
   float dist = length(lightPosEye.xyz - fPosEye.xyz);
   //compute attenuation
   float att = 1.0f / (constant + linear * dist + quadratic * (dist * dist));

	vec3 cameraPosEye = vec3(0.0f);//in eye coordinates, the viewer is situated at the origin
	
	//transform normal
	vec3 normalEye = normalize(fNormal);	
	
	//compute light direction

	vec3 lightDirN = normalize(lightPosEye.xyz - fPosEye.xyz);
	
	//compute view direction 
	vec3 viewDirN = normalize(cameraPosEye - fPosEye.xyz);
	

	//compute half vector
    vec3 halfVector = normalize(lightDirN + viewDirN);
	
	//compute specular light
	vec3 reflection = reflect(-lightDirN, normalEye);
	float specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
	

	//compute ambient light
    ambient = att * ambientStrength * lightColor;
    //compute diffuse light
    diffuse = att * max(dot(normalEye, lightDirN), 0.0f) * lightColor;
    specular = att * specularStrength * specCoeff * lightColor;

}


void main() 
{

	
	computeLightComponents();
	
	if(pointLight==true){
		pointLightFunc();
	}
	
	vec3 baseColor = vec3(0.9f, 0.35f, 0.0f);//orange
	
	ambient *= texture(diffuseTexture, fTexCoords).rgb;
	diffuse *= texture(diffuseTexture, fTexCoords).rgb;
	specular *= texture(specularTexture, fTexCoords).rgb;

	float shadow;
	shadow = computeShadow();
	vec3 color = min((ambient + (1.0f - shadow)*diffuse) + (1.0f - shadow)*specular, 1.0f);
    
	float fogFactor = computeFog();
	vec4 fogColor = vec4(0.2f, 0.2f, 0.2f, 1.0f);


	if(withFog == 1) {
		fColor = fogColor * (1 - fogFactor) + vec4(color, 1.0f) * fogFactor;
	} else {
		fColor = vec4(color, 1.0f);
	}
	 
	
}