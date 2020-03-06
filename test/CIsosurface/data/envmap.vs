include engine/ShaderVS.txt

shader_out vec2 v_texcoord;
shader_out vec3 v_color;

vec2 texgen_spheremap(vec3 view_position, vec3 view_normal)
{
	// not sure why. shouldn't be necessary when I read about how sphere mapping
	// math is specified. however, this does give me the same results as how
	// I recall it from the original/legacy OpenGL code
	view_position = -view_position;

	vec3 view_direction = normalize(view_position);
	float reflection_incidence = dot(view_direction, view_normal);
	vec3 reflection_vec = view_direction - 2.0 * reflection_incidence * view_normal;
	
	float p = sqrt(
		reflection_vec.x * reflection_vec.x +
		reflection_vec.y * reflection_vec.y +
		(reflection_vec.z + 1.0) * (reflection_vec.z + 1.0));
	
	vec2 result;
	result.x = reflection_vec.x / (2.0 * p) + 0.5;
	result.y = reflection_vec.y / (2.0 * p) + 0.5;
	
	return result;
}

void main()
{
	gl_Position = objectToProjection(unpackPosition());

	vec4 view_position = objectToView(unpackPosition());
	vec3 view_normal = objectToView3(unpackNormal().xyz);
	
	v_texcoord = texgen_spheremap(view_position.xyz, view_normal);

	//

	vec3 lightColor = vec3(1.0);
	vec3 lightDirection = normalize(vec3(0, 1, -2));
	float lightIntensity = max(0.0, dot(lightDirection, view_normal));
	
	v_color = lightColor * lightIntensity;
}
