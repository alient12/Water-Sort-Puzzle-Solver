#ifndef CONTAINER_H
#define CONTAINER_H
#include <iostream>

class Container
{
    private:
        size_t Capacity{};
        int colors[4];
        void getpoured();

    public: 
        Container(int *);
        Container(const Container&);
        size_t get_capacity();
        int get_colors(size_t);
        void pour_by(Container &);
        bool is_pourable_by(Container &);
        bool is_fool_pour(Container &);
        void show();
        bool is_full_of_one_color();
};

#endif