//
// Created by newbiewY on 2025/4/10.
//

#ifndef SOURCE_RAW_TEXTURE_DATA_H
#define SOURCE_RAW_TEXTURE_DATA_H

#include "gpupixel_context.h"

#include "../sink/sink_raw_data.h"
#include "source_raw_data.h"
#include "beauty_face_filter.h"
#include "sink_raw_texture_data.h"

namespace gpupixel {

class SourceRawTextureData : public Filter {
public:
    SourceRawTextureData();
    ~SourceRawTextureData();
    void init();
    int processData(GLuint textureId, int width, int height);

private:
    int genTextureWithTextureId(GLuint textureId,int width,int height);

private:
    std::shared_ptr<SinkRawTextureData> target_raw_output;
    GPUPixelGLProgram* _filterProgram;
    GLuint _filterPositionAttribute;
    GLuint _filterTexCoordAttribute;

    RotationMode _rotation = NoRotation;
    std::shared_ptr<GPUPixelFramebuffer> _framebuffer;
};

} // gpupiexl

#endif //SOURCE_RAW_TEXTURE_DATA_H
