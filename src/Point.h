#ifndef POINT_H
#define POINT_H

class Point
{
public:
    explicit Point(const int x = 0, const int y = 0) : xPos_(x), yPos_(y) {}
    ~Point() {}

    void setPoint(const int x, const int y)
    {
        xPos_ = x;
        yPos_ = y;
    }

    void setPoint(Point newPoint)
    {
        xPos_ = newPoint.getXPos();
        yPos_ = newPoint.getYPos();
    }

    int getXPos()
    {
        return xPos_;
    }

    int getYPos()
    {
        return yPos_;
    }




private:
    int xPos_;
    int yPos_;
}

#endif
