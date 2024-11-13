#ifndef MESH_H
#define MESH_H

#include <QVector>
#include <QVector3D>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QFile>

class Mesh
{
public:
    QOpenGLBuffer vertexBuffer;
    QOpenGLBuffer indexBuffer;
    QOpenGLBuffer normalBuffer;

    QVector<QVector3D> m_vertex;
    QVector<GLushort> m_index;
    QVector<QVector3D> m_normals;

    Mesh();
    ~Mesh();

    void init();
    void draw(QOpenGLShaderProgram *program);
    void initCube();
    bool loadMap(QImage img);
};

#endif // MESH_H
