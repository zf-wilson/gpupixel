//
// Created by newbiewY on 2025/4/14.
//

#include "sink_raw_texture_data.h"
#include "sink.h"

using namespace gpupixel;

SinkRawTextureData::SinkRawTextureData() {

}

SinkRawTextureData::~SinkRawTextureData() {

}

std::shared_ptr<SinkRawTextureData> SinkRawTextureData::create() {
    auto instance = std::shared_ptr<SinkRawTextureData>(new SinkRawTextureData());
    return instance;
}

void SinkRawTextureData::render() {
    if (_inputFramebuffers.empty()) {
        return ;
    }

    _outputTextureId = _inputFramebuffers[0].frameBuffer->getTexture();
}

GLuint SinkRawTextureData::getOutputTextureId() {
    return _outputTextureId;
}
