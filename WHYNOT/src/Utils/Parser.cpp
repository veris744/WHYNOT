#include "Parser.h"

#include "Rotation.h"

string Parser::ParseRotation(const Rotation& data)
{
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(2)
           << "(" << data.pitch << ", " << data.yaw << ", " << data.roll << ")";
    return stream.str();
}
