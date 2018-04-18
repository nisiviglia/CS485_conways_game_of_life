/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package conwaygui;

import java.util.ArrayList;
import testing.MockData_View;

/**
 *
 * @author Steve Coluccio
 */
public class CoordGrouping {
    
    //FIELDS
    private final ArrayList<Coordinate> group;
    private final GroupType type;

    
    public enum GroupType{
        SELECTION,
        CUT,
        COPY
    }
    
    
    //GETTERS & SETTERS
    public GroupType getGroupType()             { return type; }
    public ArrayList<Coordinate> getGroup()     { return group;}


    public CoordGrouping(GroupType type, ArrayList<Coordinate> group){
        this.type = type;
        this.group = group; 
    }
    
}
