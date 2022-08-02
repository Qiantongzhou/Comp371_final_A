#version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 normals;
        layout (location = 2) in vec2 aUV;
        
        uniform mat4 model_matrix;
        uniform mat4 view_matrix = mat4(1.0);
        uniform mat4 projection_matrix = mat4(1.0);
        uniform mat4 light_view_proj_matrix;
        
        out vec3 fragment_normal;
        out vec2 vertexUV;
        out vec3 fragment_position;
        out vec4 fragment_position_light_space;
        
        void main()
        {
            fragment_normal = mat3(model_matrix)* normals;
            fragment_position = vec3(model_matrix *vec4(aPos,1.0f));
            fragment_position_light_space = light_view_proj_matrix *vec4(fragment_position,1.0f);
            mat4 modelViewProjection = projection_matrix * view_matrix * model_matrix;
            gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);
            vertexUV = aUV;
        };