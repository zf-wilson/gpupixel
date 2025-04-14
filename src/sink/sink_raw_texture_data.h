//
// Created by newbiewY on 2025/4/14.
//

#ifndef SINK_RAW_TEXTURE_DATA_H
#define SINK_RAW_TEXTURE_DATA_H

#include "sink.h"

namespace gpupixel {
class SinkRawTextureData : public Sink {
 public:
    SinkRawTextureData();
    ~SinkRawTextureData();
    static std::shared_ptr<SinkRawTextureData> create();
    void render() override;
    GLuint getOutputTextureId();

private:
    GLuint _outputTextureId = 0;
};

}

#endif //SINK_RAW_TEXTURE_DATA_H
