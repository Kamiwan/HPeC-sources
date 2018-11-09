#ifndef COMM_SHARED_MEMORY_H
#define COMM_SHARED_MEMORY_H

class Box {
    private:
        double length;   // Length of a box
        double breadth;  // Breadth of a box
        double height;   // Height of a box
        
    public:
        Box(int a, int b, int c)
        {
            length  = a;
            breadth = b;
            height  = c;
        }
        void print();
};

#endif