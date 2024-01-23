#include <map>
#include <string>
#include <glad/glad.h>

namespace shaders {
    class ShaderManager {
        private:
            std::map<std::string, GLuint> programs;
            std::string activeProgram;
            
        public:
            void LoadShader(const char * filename, GLuint shader_id);
            static GLuint CreateGpuProgram(GLuint vertex_shader_id, GLuint fragment_shader_id);
            GLuint LoadShader_Vertex(const char* filename);
            GLuint LoadShader_Fragment(const char* filename);
            void LoadProgram(std::string name, std::string vertexShader,
                            std::string fragmentShader);
            void UseProgram(std::string name);
            GLuint GetActiveProgram();
    };
}