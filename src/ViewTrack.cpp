//
//  main.cpp
//
//  Copyright © 2018 Alun Evans. All rights reserved.
//
#include "components.h"
#include "extern.h"
#include "Game.h"
#include "Curve.h"

ViewTrack::ViewTrack()
{
    ratio = 0;
}

// Generate a mesh made of lines
void ViewTrack::GenerateMesh()
{
    // Get our viewtracks and set a VBO 
    std::vector<float> track_vertices;
    std::vector<GLuint> track_indices;

    int index = 0;
    auto& view_tracks = ECS.getAllComponents<ViewTrack>();

    for (auto& cc : view_tracks) {

        // We have a path track
        if (cc.curve._knots.size() > 1)
        {
            for (unsigned int i = 0; i < cc.curve._knots.size(); i++)
            {
                track_vertices.push_back(cc.curve._knots[i].x);
                track_vertices.push_back(cc.curve._knots[i].y);
                track_vertices.push_back(cc.curve._knots[i].z);

                //if (i < cc.knots.size() - 1)
                {
                    track_indices.push_back(index + 3 * i);
                    track_indices.push_back(index + 3 * i + 1);
                    track_indices.push_back(index + 3 * i + 1);
                    track_indices.push_back(index + 3 * i + 2);
                    track_indices.push_back(index + 3 * i + 2);
                    track_indices.push_back(index + 3 * i + 3);
                }
            }
        }

        index += cc.curve._knots.size() - 1;

        GLuint* track_line_indices = &track_indices[0]; // Hardcoded size, we might have some limitations here...

        //gl buffers
        glGenVertexArrays(1, &curve_vao_);
        glBindVertexArray(curve_vao_);

        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, (track_indices.size()) * sizeof(float), &(track_vertices[0]), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        //indices
        GLuint ibo;
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, track_indices.size() * sizeof(GLuint), &(track_line_indices[0]), GL_STATIC_DRAW);

        //unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}
