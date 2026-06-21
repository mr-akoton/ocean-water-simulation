#ifndef RBO_HPP
#define RBO_HPP

#include <glad/glad.h>

class RBO {
private:
  GLuint _id;

public:
  RBO(int width, int height);
  ~RBO();

  void bind(void) const;
  void unbind(void) const;
};

#endif
