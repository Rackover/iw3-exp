#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace Utils
{
    namespace Image
    {

        struct vec3 {
            float r;
            float g;
            float b;

            vec3(float x, float y, float z)
            {
                this->r = x;
                this->g = y;
                this->b = z;
            }

            vec3(float a)
            {
                r = a;
                g = a;
                b = a;
            }
        };

        void GaussianBlur4(unsigned char* data, unsigned char* newData, int width, int height, int channelCount, int radius);
    }
} 
