#include "mesh.h"
#include <QPixmap>
#include <QColorSpace>
#include <iostream>
Mesh::Mesh() : vertexBuffer(QOpenGLBuffer::VertexBuffer), indexBuffer(QOpenGLBuffer::IndexBuffer){}

Mesh::~Mesh(){
    vertexBuffer.destroy();
    indexBuffer.destroy();
}

void Mesh::init(){
    m_vertex = {};
    m_index = {};
    m_normals = {};

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
}
bool Mesh::loadMap(QImage img){
    QImage image = img.scaled(200,200);
    m_vertex.clear();

    for(long i = 0; i<image.width(); i++){
        for(long j = 0; j<image.height(); j++){
            QVector3D vertex((i-image.width()/2.0),
                 -image.pixelColor(i,j).black()-128,
                 j-image.height()/2.0);
            vertex = vertex / image.width() * 10.0;
            m_vertex.append(vertex);
        }
    }


    // Initialize normals as zero vectors
    m_normals.fill(QVector3D(0.0f, 0.0f, 0.0f), m_vertex.size());

    m_index.clear();
    for(long i = 0; i<image.width()-1; i++){
        for(long j = 0; j<image.height()-1; j++){
            m_index.append(i*image.width()+j);
            m_index.append(i*image.width()+(j+1));
            m_index.append((i+1)*image.width()+j);

            m_index.append((i+1)*image.width()+j+1);
            m_index.append((i+1)*image.width()+(j));
            m_index.append(i*image.width()+j+1);
        }
    }


    // Compute normals for each face
    for (int i = 0; i < m_index.size(); i += 3) {
        GLuint idx0 = m_index[i];
        GLuint idx1 = m_index[i + 1];
        GLuint idx2 = m_index[i + 2];

        QVector3D v0 = m_vertex[idx0];
        QVector3D v1 = m_vertex[idx1];
        QVector3D v2 = m_vertex[idx2];

        QVector3D normal = QVector3D::crossProduct(v1 - v0, v2 - v0).normalized();

        m_normals[idx0] += normal;
        m_normals[idx1] += normal;
        m_normals[idx2] += normal;
    }

    // Normalize normals
    for (int i = 0; i < m_normals.size(); ++i) {
        m_normals[i] = m_normals[i].normalized();
    }

    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.allocate(m_vertex.constData(), m_vertex.size() * sizeof(QVector3D));
    vertexBuffer.release();

    indexBuffer.create();
    indexBuffer.bind();
    indexBuffer.allocate(m_index.constData(), m_index.size() * sizeof(GLuint));
    indexBuffer.release();

    normalBuffer.create();
    normalBuffer.bind();
    normalBuffer.allocate(m_normals.constData(), m_normals.size() * sizeof(QVector3D));
    normalBuffer.release();
    return true;
}
