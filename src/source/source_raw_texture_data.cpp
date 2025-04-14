//
// Created by newbiewY on 2025/4/10.
//

#include "source_raw_texture_data.h"
#include "source_raw_data.h"
#include "smooth_toon_filter.h"

namespace gpupixel {

    const std::string kI420VertexShaderString = R"(
    attribute vec4 position;
    attribute vec4 inputTextureCoordinate;
    varying vec2 textureCoordinate;

    void main() {
      textureCoordinate = (inputTextureCoordinate).xy;
      gl_Position = position;
    })";

#if defined(GPUPIXEL_IOS) || defined(GPUPIXEL_ANDROID)
    const std::string kI420FragmentShaderString = R"(
    varying mediump vec2 textureCoordinate; uniform sampler2D yTexture;
    uniform sampler2D uTexture;
    uniform sampler2D vTexture;
    uniform sampler2D inputImageTexture;
    uniform int texture_type;
    mediump mat3 trans =
        mat3(1.0, 1.0, 1.0,
             0, -0.34414, 1.772,
             1.402, -0.71414, 0);

    void main() {
      mediump vec3 yuv;
      if (texture_type == 0) {  // yuv
        yuv.x = texture2D(yTexture, textureCoordinate).r;
        yuv.y = texture2D(uTexture, textureCoordinate).r - 0.5;
        yuv.z = texture2D(vTexture, textureCoordinate).r - 0.5;

        gl_FragColor = vec4(trans * yuv, 1.0);
      } else {
        gl_FragColor = texture2D(inputImageTexture, textureCoordinate);
      }
    })";
#elif defined(GPUPIXEL_MAC) || defined(GPUPIXEL_WIN) || defined(GPUPIXEL_LINUX)
    const std::string kI420FragmentShaderString = R"(
    varying vec2 textureCoordinate; uniform sampler2D yTexture;
    uniform sampler2D uTexture;
    uniform sampler2D vTexture;
    uniform sampler2D inputImageTexture;
    uniform int texture_type;
    mat3 trans = mat3(1.0, 1.0, 1.0,
                      0, -0.34414, 1.772,
                      1.402, -0.71414, 0);

    void main() {
      vec3 yuv;
      if (texture_type == 0) {  // i420
        yuv.x = texture2D(yTexture, textureCoordinate).r;
        yuv.y = texture2D(uTexture, textureCoordinate).r - 0.5;
        yuv.z = texture2D(vTexture, textureCoordinate).r - 0.5;

        gl_FragColor = vec4(trans * yuv, 1.0);
      } else {
        gl_FragColor = texture2D(inputImageTexture, textureCoordinate);
      }
    })";
#endif

void SourceRawTextureData::init() {
    _filterProgram = GPUPixelGLProgram::createByShaderString(kI420VertexShaderString,
                                                             kI420FragmentShaderString);
    GPUPixelContext::getInstance()->setActiveShaderProgram(_filterProgram);

    _filterPositionAttribute = _filterProgram->getAttribLocation("position");
    _filterTexCoordAttribute =
            _filterProgram->getAttribLocation("inputTextureCoordinate");
}

int SourceRawTextureData::processData(GLuint texture, int width, int height) {
    return genTextureWithTextureId(texture, width, height);
}

int SourceRawTextureData::genTextureWithTextureId(GLuint texture, int width, int height) {
    if (!_framebuffer || (_framebuffer->getWidth() != width ||
                          _framebuffer->getHeight() != height)) {
        _framebuffer =
                GPUPixelContext::getInstance()->getFramebufferFactory()->fetchFramebuffer(
                        width, height);
    }
    this->setFramebuffer(_framebuffer, NoRotation);

    GPUPixelContext::getInstance()->setActiveShaderProgram(_filterProgram);
    this->getFramebuffer()->active();
    GLfloat imageVertices[]{
            -1.0f, -1.0f,
            1.0f, -1.0f,
            -1.0f, 1.0f,
            1.0f, 1.0f,
    };

    _filterProgram->setUniformValue("texture_type", 1);
    CHECK_GL(glEnableVertexAttribArray(_filterPositionAttribute));
    CHECK_GL(glVertexAttribPointer(_filterPositionAttribute, 2, GL_FLOAT, 0, 0,
                                   imageVertices));

    CHECK_GL(glEnableVertexAttribArray(_filterTexCoordAttribute));
    CHECK_GL(glVertexAttribPointer(_filterTexCoordAttribute, 2, GL_FLOAT, 0, 0,
                                   _getTexureCoordinate(_rotation)));

    CHECK_GL(glActiveTexture(GL_TEXTURE0));
    CHECK_GL(glBindTexture(GL_TEXTURE_2D, texture));
    _filterProgram->setUniformValue("inputImageTexture", 0);
    // draw frame buffer
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    this->getFramebuffer()->inactive();
    Source::doRender(true);
    return 0;
}

SourceRawTextureData::SourceRawTextureData() {
    init();
}

SourceRawTextureData::~SourceRawTextureData() {

}

} // gpupiexl
