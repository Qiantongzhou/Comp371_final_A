#version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 normals;
        layout (location = 2) in vec2 aUV;
        layout (location = 3) in vec3 aColor;
        
        uniform mat4 worldMatrix;
        uniform mat4 viewMatrix = mat4(1.0);
        uniform mat4 projectionMatrix = mat4(1.0);
        uniform mat4 lightMatrix;
        
        out vec3 fragment_normal;
        out vec2 vertexUV;
        out vec3 fragment_position;
        out vec4 fragment_position_light_space;
        out vec3 vertexColor;
        
        void main()
        {
        	fragment_normal = mat3(worldMatrix)* normals;
        	fragment_position = vec3(worldMatrix *vec4(aPos,1.0f));
        	fragment_position_light_space = lightMatrix *vec4(fragment_position,1.0f);
          mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;
          gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);
          vertexUV = aUV;
        
          vertexColor = aColor;
        };