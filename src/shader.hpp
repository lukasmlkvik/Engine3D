#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "types.hpp"

class ShaderModule {
    public:
    ui32 shaderModuleId;

    ShaderModule(const std::string& filePath, ui32 moduleType){
        
        shaderModuleId = glCreateShader(moduleType);
        init(filePath, moduleType);
    }
    bool init(const std::string& filePath, ui32 moduleType){
        std::ifstream file;
        std::stringstream bufferedLines;
        std::string line;

        file.open(filePath);
        while (std::getline(file, line)) {
            bufferedLines << line << '\n';
        }

        std::string shaderSource = bufferedLines.str();
        const char* shaderSrc = shaderSource.c_str();
        bufferedLines.str("");
        file.close();

        glShaderSource(shaderModuleId, 1, &shaderSrc, NULL);
        glCompileShader(shaderModuleId);

        int success;
        glGetShaderiv(shaderModuleId, GL_COMPILE_STATUS, &success);
        if (success == 0) {
            char errorLog[1024];
            glGetShaderInfoLog(shaderModuleId, 1024, NULL, errorLog);
            std::cout << "Shader Module compilation error:\n" << errorLog << std::endl;
        }
        return success != 0;
    }

    ~ShaderModule() {
		glDeleteShader(shaderModuleId);
    }
};

class Shader {
    public:
    ui32 shaderId;
    std::vector<ShaderModule *> modules;
    Shader(){
        shaderId = glCreateProgram();
    }

    unsigned int init(const std::vector<ShaderModule *>& modules) {

        this->modules = modules;

        //Attach all the modules then link the program
        for (ShaderModule * shaderModule : modules) {
            glAttachShader(shaderId, shaderModule->shaderModuleId);
        }
        glLinkProgram(shaderId);

        int success;
        glGetProgramiv(shaderId, GL_LINK_STATUS, &success);
        if (success == 0) {
            char errorLog[1024];
            glGetProgramInfoLog(shaderId, 1024, NULL, errorLog);
            std::cout << "Shader linking error:\n" << errorLog << '\n';
        }
        return success != 0;
    }
    void bind(){
        glUseProgram(shaderId);
    }
    ~Shader(){
        glDeleteProgram(shaderId);
    }
};