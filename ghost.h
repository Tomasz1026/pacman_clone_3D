#pragma once

#define SPEED 3.0f

class Ghost {     
public:           

    float x = 0;
    float z = 0;
    
    std::vector <float> vert;
    std::vector <float> norm;
    std::vector <float> texCo;

    GLuint tex_in_use{}, tex{}, tex_run_away{}, vbo[3]{};
   
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    float angle = 0;
    float angle_speed = PI;
    float y_angle = 0.0f;
    bool consequence = false;
    bool run_away = false;

    unsigned __int16 px_on_map = 0;
    unsigned __int16 pz_on_map = 0;

    Ghost() {     
        //printf("Hello World!\n");
    }
    ~Ghost() {
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

        tex_in_use = tex;
    }

    void move() {

        angle += angle_speed * (float)glfwGetTime();

        if (angle >= PI) {
            angle = 0.0f;
        }

        if (up) {
            z += SPEED * (float)glfwGetTime();
        }
        if (down) {
            z -= SPEED * (float)glfwGetTime();
        }
        if (right) {
            x -= SPEED * (float)glfwGetTime();
        }
        if (left) {
            x += SPEED * (float)glfwGetTime();
        }
    }

    void draw(ShaderProgram* sp) {

        glm::mat4 Mt = glm::mat4(1.0f);
        Mt = glm::translate(Mt, glm::vec3(x, 0.0f, z));
        Mt = glm::rotate(Mt, angle, glm::vec3(0.0f, 1.0f, 0.0f));

        glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(Mt));


        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glEnableVertexAttribArray(sp->a("vertex"));
        glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, 0);
        
        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glEnableVertexAttribArray(sp->a("normal"));
        glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
        glEnableVertexAttribArray(sp->a("texCoord0"));
        glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, 0);


        glUniform1i(sp->u("textureMap0"), 1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, tex_in_use);

        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vert.size());
    }

    void collision(int map[][70], int player_x, int player_z) {
        px_on_map = (unsigned __int16)floor(x / -0.2f);
        pz_on_map = (unsigned __int16)floor(z / -0.2f);
        
        if (map[pz_on_map][px_on_map] == 2) {
            consequence = false;
        }

        if (map[pz_on_map][px_on_map] > 4 && map[pz_on_map][px_on_map] < 7 && fabs(z - pz_on_map * -0.2f) < 0.09f && fabs(x - px_on_map * -0.2f) < 0.09f && !consequence)
        {
            std::vector <short> options;//0 - left, 1- up, 2 - right, 3 - down
            //printf("Thinking...\n");
            consequence = true;

            bool op[4] = { 0,0,0,0 };
            
            if (map[pz_on_map][px_on_map + 1] == 2 && !left)
            {
                options.push_back(2);
                op[2] = 1;
            }
            if (map[pz_on_map][px_on_map - 1] == 2 && !right)
            {
                options.push_back(0);
                op[0] = 1;
            }
            if (map[pz_on_map + 1][px_on_map] == 2 && !up)
            {
                options.push_back(3);
                op[3] = 1;
            }
            if (map[pz_on_map - 1][px_on_map] == 2 && !down)
            {
                options.push_back(1);
                op[1] = 1;
            }

            int what_next = options[rand() % options.size()];

            if (run_away) {
                if (abs(px_on_map + 1 - player_x) > abs(px_on_map - player_x) && op[2]) {
                    what_next = 2;//right
                }
                else if (abs(px_on_map - 1 - player_x) > abs(px_on_map - player_x) && op[0]) {
                    what_next = 0;//left
                }
                else if (abs(pz_on_map + 1 - player_z) > abs(pz_on_map - player_z) && op[3]) {
                    what_next = 3;//down
                }
                else if (abs(pz_on_map - 1 - player_z) > abs(pz_on_map - player_z) && op[1]) {
                    what_next = 1;//up
                }
            }
            else {
                if (abs(px_on_map + 1 - player_x) < abs(px_on_map - player_x) && op[2]) {
                    what_next = 2;//right
                }
                else if (abs(px_on_map - 1 - player_x) < abs(px_on_map - player_x) && op[0]) {
                    what_next = 0;//left
                }
                else if (abs(pz_on_map + 1 - player_z) < abs(pz_on_map - player_z) && op[3]) {
                    what_next = 3;//down
                }
                else if (abs(pz_on_map - 1 - player_z) < abs(pz_on_map - player_z) && op[1]) {
                    what_next = 1;//up
                }
            }
            

            switch (what_next) {
            case 0:
                //printf("I will go left!\n");
                left = true;
                right = false;
                up = false;
                down = false;
                break;
            case 1:
                //printf("I will go up!\n");
                left = false;
                right = false;
                up = true;
                down = false;
                break;
            case 2:
                //printf("I will go right!\n");
                left = false;
                right = true;
                up = false;
                down = false;
                break;
            case 3:
                //printf("I will go down!\n");
                left = false;
                right = false;
                up = false;
                down = true;
                break;
            }
        }

        if (map[pz_on_map][px_on_map + 1] == 0 && fabs(x - px_on_map * -0.2f) < 0.09f)
        {
            right = false;
        }
        if (map[pz_on_map][px_on_map - 1] == 0 && fabs(x - px_on_map * -0.2f) < 0.09f)
        {
            left = false;
        }
        if (map[pz_on_map + 1][px_on_map] == 0 && fabs(z - pz_on_map * -0.2f) < 0.09f)
        {
            down = false;
        }
        if (map[pz_on_map - 1][px_on_map] == 0 && fabs(z - pz_on_map * -0.2f) < 0.09f)
        {
            up = false;
        }
    }

    void start_run_away() {
        tex_in_use = tex_run_away;
        run_away = true;
    }

    void end_run_away() {
        tex_in_use = tex;
        run_away = false;
    }

    void reset(float x_pos, float z_pos) {
        x = x_pos;
        z = z_pos;
        run_away = false;
        consequence = false;
        up = false;
        down = false;
        left = false;
        right = false;
        tex_in_use = tex;
    }
};