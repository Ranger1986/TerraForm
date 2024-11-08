#include "mesh.h"
#include "lodepng.h"
#include <QPixmap>
#include <QColorSpace>
#include <iostream>
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
bool Mesh::loadOff(QString filename ){
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open file:" << filename;
        return false;
    }
    //lodepng_decode_file();
    /*
    QTextStream in(&file);
    QString line = in.readLine();
    if (line != "OFF") {
        qWarning() << "File is not in OFF format";
        return false;
    }

    // Read vertices and faces count
    line = in.readLine();
    QStringList counts = line.split(' ');
    if (counts.size() < 3) {
        qWarning() << "Invalid file format";
        return false;
    }

    int numVertices = counts[0].toInt();
    int numFaces = counts[1].toInt();

    // Read vertices
    m_vertex.clear();
    for (int i = 0; i < numVertices; ++i) {
        line = in.readLine();
        QStringList vertexData = line.split(' ');
        if (vertexData.size() != 3) {
            qWarning() << "Invalid vertex data";
            return false;
        }
        QVector3D vertex(vertexData[0].toFloat(), vertexData[1].toFloat(), vertexData[2].toFloat());
        m_vertex.append(vertex);
    }

    // Initialize normals as zero vectors
    m_normals.fill(QVector3D(0.0f, 0.0f, 0.0f), m_vertex.size());

    // Read faces
    m_index.clear();
    for (int i = 0; i < numFaces; ++i) {
        line = in.readLine();
        QStringList faceData = line.split(' ');
        if (faceData.size() < 4) {
            qWarning() << "Invalid face data";
            return false;
        }

        int numVerticesInFace = faceData[0].toInt();
        if (numVerticesInFace != 3) {
            qWarning() << "Only triangles are supported";
            return false;
        }

        for (int j = 1; j <= 3; ++j) {
            m_index.append(faceData[j].toUInt());
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

    // Load data into buffers
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
    */

    return true;
}
bool Mesh::loadMap(QPixmap img){
    QImage image = img.toImage();
    image = image.convertToFormat(QImage::Format_Grayscale8);

    m_vertex.clear();
    for(long i = 0; i<image.width(); i++){
        for(long j = 0; j<image.height(); j++){
            QVector3D vertex((i-image.width()/2),-image.pixelColor(i,j).black()-128,j-image.height()/2);
            vertex = vertex /image.width()*10;
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
