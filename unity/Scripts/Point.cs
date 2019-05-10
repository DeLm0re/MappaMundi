using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Point
{
    private int xPoint;

    private int yPoint;

    private Definition.pointEnum pointValue;

    public Point()
    {
        this.xPoint = -1;
        this.yPoint = -1;
        this.pointValue = Definition.pointEnum.UNDEFINED;
    }

    public int getXPoint()
    {
        return this.xPoint;
    }

    public void setXPoint(int xValue)
    {
        this.xPoint = xValue;
    }

    public int getYPoint()
    {
        return this.yPoint;
    }

    public void setYPoint(int yValue)
    {
        this.yPoint = yValue;
    }

    public Definition.pointEnum getPointValue()
    {
        return this.pointValue;
    }

    public void setPointValue(Definition.pointEnum aPointValue)
    {
        this.pointValue = aPointValue;
    }
}
