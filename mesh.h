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
    QOpenGLBuffer texCoordBuffer;

    QVector<QVector3D> m_vertex;
    QVector<GLushort> m_index;
    QVector<QVector3D> m_normals;
    QVector<QVector2D> m_texCoord;

    Mesh();
    ~Mesh();

    void init();
    void draw(QOpenGLShaderProgram *program);
    bool loadMap(QImage img);
};

#endif // MESH_H
