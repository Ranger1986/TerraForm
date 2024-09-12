#include "mesh.h"

Mesh::Mesh() : vertexBuffer(QOpenGLBuffer::VertexBuffer), indexBuffer(QOpenGLBuffer::IndexBuffer){}

Mesh::~Mesh(){
    vertexBuffer.destroy();
    indexBuffer.destroy();
}

void Mesh::init(){
    initCube();
}
void Mesh::initCube(){
    m_vertex = {
        QVector3D(-0.5f, -0.5f, -0.5f),
        QVector3D( 0.5f, -0.5f, -0.5f),
        QVector3D( 0.5f,  0.5f, -0.5f),
        QVector3D(-0.5f,  0.5f, -0.5f),
        QVector3D(-0.5f, -0.5f,  0.5f),
        QVector3D( 0.5f, -0.5f,  0.5f),
        QVector3D( 0.5f,  0.5f,  0.5f),
        QVector3D(-0.5f,  0.5f,  0.5f)
    };
    m_index = {
        1, 0, 2, 3, 2, 0,
        4, 5, 6, 6, 7, 4,
        0, 4, 7, 7, 3, 0,
        5, 1, 6, 2, 6, 1,
        2, 3, 6, 7, 6, 3,
        0, 1, 5, 5, 4, 0
    };
    m_normals = {
        QVector3D(-0.5f, -0.5f, -0.5f),
        QVector3D( 0.5f, -0.5f, -0.5f),
        QVector3D( 0.5f,  0.5f, -0.5f),
        QVector3D(-0.5f,  0.5f, -0.5f),
        QVector3D(-0.5f, -0.5f,  0.5f),
        QVector3D( 0.5f, -0.5f,  0.5f),
        QVector3D( 0.5f,  0.5f,  0.5f),
        QVector3D(-0.5f,  0.5f,  0.5f)
    };

    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.allocate(m_vertex.constData(), m_vertex.size()* sizeof(QVector3D));
    vertexBuffer.release();

    indexBuffer.create();
    indexBuffer.bind();
    indexBuffer.allocate(m_index.constData(), m_index.size() * sizeof(GLushort));
    indexBuffer.release();

    normalBuffer.create();
    normalBuffer.bind();
    normalBuffer.allocate(m_normals.constData(), m_normals.size()* sizeof(QVector3D));
    normalBuffer.release();
}

void Mesh::draw(QOpenGLShaderProgram *program)
{
    // program->bind();

    vertexBuffer.bind();
    int vertexLocation = program->attributeLocation("vertex");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));

    indexBuffer.bind();
    glDrawElements(GL_TRIANGLES, m_index.size(), GL_UNSIGNED_SHORT, 0);

    normalBuffer.bind();
    int normalLocation = program->attributeLocation("normal");
    program->enableAttributeArray(normalLocation);
    program->setAttributeBuffer(normalLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));

    vertexBuffer.release();
    indexBuffer.release();
    normalBuffer.release();
    //program->release();
}
