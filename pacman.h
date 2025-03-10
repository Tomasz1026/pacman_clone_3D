#pragma once

#define SPEED 4.0f

class PacMan {
public:
    float x = 35 * -0.2f;
    float z = 66 * -0.2f;

    std::vector <float> vert;
    std::vector <float> norm;
    std::vector <float> texCo;
    GLuint tex{}, vbo[3]{};

    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    char next_move = 4;
    float angle = 0;
    float angle_speed = 3*PI;
    float y_angle = 0.0f;
    int score = 0;
    bool sound_type = true;
    bool power_up = false;
    bool chase = false;
    short lives = 3;

    unsigned __int16 px_on_map = 0;
    unsigned __int16 pz_on_map = 0;

    PacMan() {
        //printf("Hello World!\n");
    }
    ~PacMan() {
        glUnmapNamedBuffer(vbo[0]);
        glUnmapNamedBuffer(vbo[1]);
        glUnmapNamedBuffer(vbo[2]);
        //printf("Auf wiedersehen!");
    }

    void init() {
        glGenBuffers(3, vbo);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, (vert.size()) * sizeof(float), &vert[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, (norm.size()) * sizeof(float), &norm[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
        glBufferData(GL_ARRAY_BUFFER, (texCo.size()) * sizeof(float), &texCo[0], GL_STATIC_DRAW);
    }

    void move() {

        angle += angle_speed * (float)glfwGetTime();

        if (angle >= 1.0f || angle <= -0.0f) {
            angle_speed = -angle_speed;
        }

        if (angle >= 1.0f) {
            angle = 1.0f;
        }
        if (angle <= -0.0f) {
            angle = -0.0f;
        }

        if (up) {
            z += SPEED * (float)glfwGetTime();
            y_angle = -PI / 2;
        }
        if (down) {
            z -= SPEED * (float)glfwGetTime();
            y_angle = PI / 2;
        }
        if (right) {
            x -= SPEED * (float)glfwGetTime();
            y_angle = PI;

        }
        if (left) {
            x += SPEED * (float)glfwGetTime();
            y_angle = 0.0f;
        }
    }

    void draw(ShaderProgram* sp, glm::mat4 Mt) {
        Mt = glm::translate(Mt, glm::vec3(x, 0.0f, z));

        Mt = glm::rotate(Mt, y_angle, glm::vec3(0.0f, 1.0f, 0.0f));
        Mt = glm::rotate(Mt, angle, glm::vec3(0.0f, 0.0f, 1.0f));

        glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(Mt));

        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
        glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, 0);

        glUniform1i(sp->u("textureMap0"), 0); // Associate sampler textureMap0 with the 0-th texturing unit
        glActiveTexture(GL_TEXTURE0); //Assign texture tex0 to the 0-th texturing unit
        glBindTexture(GL_TEXTURE_2D, tex);

        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vert.size()); //upper half

        Mt = glm::mat4(1.0f);
        Mt = glm::translate(Mt, glm::vec3(x, 0.0f, z));
        Mt = glm::rotate(Mt, y_angle, glm::vec3(0.0f, 1.0f, 0.0f));
        Mt = glm::rotate(Mt, -angle, glm::vec3(0.0f, 0.0f, 1.0f));
        Mt = glm::rotate(Mt, PI, glm::vec3(0.0f, 0.0f, 1.0f));

        glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(Mt));

        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vert.size()); //lower half
    }

    void collision(int map[][70], glm::vec4 lights[210]) {
        px_on_map = (unsigned __int16)floor(x / -0.2f);
        pz_on_map = (unsigned __int16)floor(z / -0.2f);

        //can i move there?
        if ((map[pz_on_map][px_on_map + 1] > 1 && map[pz_on_map][px_on_map + 1] < 9 && fabs(z - pz_on_map * -0.2f) < 0.07f) && next_move == 2)
        {
            left = false;
            right = true;
            up = false;
            down = false;
            next_move = 4;
        }
        else if ((map[pz_on_map][px_on_map - 1] > 1 && map[pz_on_map][px_on_map - 1] < 9 && fabs(z - pz_on_map * -0.2f) < 0.07f) && next_move == 0)
        {
            left = true;
            right = false;
            up = false;
            down = false;
            next_move = 4;
        }
        else if ((map[pz_on_map + 1][px_on_map] > 1 && map[pz_on_map + 1][px_on_map] < 9 && fabs(x - px_on_map * -0.2f) < 0.07f) && next_move == 3)
        {
            left = false;
            right = false;
            up = false;
            down = true;
            next_move = 4;
        }
        else if ((map[pz_on_map - 1][px_on_map] > 1 && map[pz_on_map - 1][px_on_map] < 9 && fabs(x - px_on_map * -0.2f) < 0.07f) && next_move == 1)
        {
            left = false;
            right = false;
            up = true;
            down = false;
            next_move = 4;
        }

        //eating candy or power up
        if (map[pz_on_map][px_on_map] > 2 && map[pz_on_map][px_on_map] < 6 && fabs(x - px_on_map * -0.2f) < 0.07f) {
            
            if (chase == false) {
                if (sound_type) {
                    PlaySound(TEXT("chomp_1.wav"), NULL, SND_ASYNC | SND_FILENAME);
                }
                else {
                    PlaySound(TEXT("chomp_2.wav"), NULL, SND_ASYNC | SND_FILENAME);
                }
            }
            
            if (map[pz_on_map][px_on_map] == 5) {
                map[pz_on_map][px_on_map] = 6;
            }
            else if (map[pz_on_map][px_on_map] == 4) { //eats power up
                map[pz_on_map][px_on_map] = 2;

                power_up = true;
                chase = true;
            }
            else {
                map[pz_on_map][px_on_map]--;
            }

            score++;
            sound_type = !sound_type;
        }

        //portal mechanism
        if (map[pz_on_map][px_on_map] == 9 && fabs(x - px_on_map * -0.2f) < 0.07f) {
            if (left) {
                x = 69 * -0.2f;
            }
            else if (right) {
                x = 1 * -0.2f;
            }
        }

        //walls collision
        if (map[pz_on_map][px_on_map + 1] == 0 && fabs(x - px_on_map * -0.2f) < 0.07f)
        {
            right = false;
        }
        if (map[pz_on_map][px_on_map - 1] == 0 && fabs(x - px_on_map * -0.2f) < 0.07f)
        {
            left = false;
        }
        if (map[pz_on_map + 1][px_on_map] == 0 && fabs(z - pz_on_map * -0.2f) < 0.07f)
        {
            down = false;
        }
        if (map[pz_on_map - 1][px_on_map] == 0 && fabs(z - pz_on_map * -0.2f) < 0.07f)
        {
            up = false;
        }
    }

    void reset() {
        x = 35.0f * -0.2f;
        z = 66.0f * -0.2f;
        score = 0;
        lives = 3;
        left = true;
        right = false;
        up = false;
        down = false;
    }
};
