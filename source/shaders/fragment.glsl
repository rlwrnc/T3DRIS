#version 330 core
in vec3 fragment_position;
in vec3 normal;

out vec4 color;

uniform vec4 in_color;
uniform vec3 view_position;
uniform vec3 light_position;

void main()
{
	vec3 light_color = vec3(1.0, 1.0, 1.0);

	float ambient_strength = 0.1;
	vec3 ambient = ambient_strength * light_color;

	vec3 norm = normalize(normal);
	vec3 light_direction = normalize(light_position - fragment_position);
	float diff = max(dot(norm, light_direction), 0.0);
	vec3 diffuse = diff * light_color;

	float specular_intensity = 0.5;
	vec3 view_direction = normalize(view_position - fragment_position);
	vec3 reflect_direction = reflect(-light_direction, norm);
	float spec = pow(max(dot(view_direction, reflect_direction), 0.0), 32);
	vec3 specular = specular_intensity * spec * light_color;

	vec3 result = (ambient + diffuse + specular) * in_color.xyz;
	color = vec4(result, 1.0);
}
