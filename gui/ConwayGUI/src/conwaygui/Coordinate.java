/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package conwaygui;

/**
 *
 * @author saynt
 */
public class Coordinate {
    
    public int x,y;
        
    public Coordinate(int x, int y){
        this.x = x;
        this.y = y;
    }

    @Override
    public boolean equals(Object obj){
        if (!Coordinate.class.isAssignableFrom(obj.getClass())) {
            return false;
        }
        final Coordinate c = (Coordinate) obj;

        return c.x == x && c.y == y; 
    }

    //Necessary for overriding equals
    @Override
    public int hashCode() {
        int hash = 7;
        hash = 41 * hash + this.x;
        hash = 41 * hash + this.y;
        return hash;
    }

    @Override
    public String toString(){
        return "Coordinate: " + x + "," + y;   
    }

    public Coordinate copy(){
        return new Coordinate(x,y);
    }
    
}
