#ifndef EBO_HPP
#define EBO_HPP

#include <glad/glad.h>
#include <vector>

class EBO {
private:
  GLuint _id;

public:
  EBO(void);
  EBO(const std::vector<GLuint>& indices);
  ~EBO();

  void bindData(const std::vector<GLuint>& indices);
  void bind(void) const;
  void unbind(void) const;
  GLuint getID(void) const;
};

#endif
