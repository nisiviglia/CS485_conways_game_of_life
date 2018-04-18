/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package testing;

import com.google.gson.Gson;
import com.google.gson.JsonArray;
import com.google.gson.JsonObject;
import conwaygui.ApplicationManager;
import conwaygui.Coordinate;
import java.util.ArrayList;

/**
 *
 * @author saynt
 */
public class MockData_View {
    
    //mock region starting at (0,0)
    //NOTE: These dimensions should always be power of two for simplicity in rendering
    
    public static class View{
        
        public ArrayList<Coordinate> livingCells;
        public int origin_x; 
        public int origin_y;
        public int width;
        public int height;
        
        private ArrayList<Coordinate> originalCells = new ArrayList<Coordinate>();;
        
        public View(int x, int y, int w, int h){
            origin_x = x;
            origin_y = y;
            width = w;
            height = h;
            livingCells = new ArrayList<Coordinate>();
        }
        
        //Default Constructor
        //Typically called for creatine new grid/render view
        public View(){
            this(0,0,128,128);
        }
        
        public View(ArrayList<Coordinate> livingCellsFromFrame){
            this();
            for(Coordinate c : livingCellsFromFrame){
                originalCells.add(c);
                livingCells.add(c);
            }
        }
        
        //Have cells been changed in this view
        public boolean isDirty(){ 
            return !originalCells.equals(livingCells); 
        }
        
        public boolean getState(Coordinate coord){
            return livingCells.contains(coord);
        }
        
        //Change cell to specified state
        public void changeCell(Coordinate c, boolean alive){
            if(!alive){
                livingCells.remove(c);
            }else if(!livingCells.contains(c)){
                livingCells.add(c);
            }
        }
        
        //Flip the state of the specified cell
        public void flipCell(Coordinate c){
            changeCell(c, !getState(c));
        }
        
        public void wipeCells(){
            livingCells.clear();
        }
    }
    
    
    
    /**********************************
     * 
     *       MOCK JSON INFO
     * 
    **********************************/
    public  static String testJson1 = 
            "{\"title\":\"Point\", \"properties\":{\"x\":3, \"y\": 5}}";
            //"{\"test\":10}";
    
    
    
    static Gson gson = new Gson();

    public static String createJsonString(){
        
        JsonObject testFrame = new JsonObject();
        testFrame.addProperty("success", true);
        testFrame.addProperty("world_name", ApplicationManager.currentGridName);
        testFrame.addProperty("frame_id", 1);
        JsonArray testCells = new JsonArray();
        for(int i = 0; i < 20; i++){
            JsonObject p1 = new JsonObject();
            p1.addProperty("x", 70 + i);
            p1.addProperty("y", 70 + i);
            testCells.add(p1.deepCopy());
        }
        testFrame.add("live_cells", testCells);
        
        return gson.toJson(testFrame);
    }
    
    
    
    
    /*
    public static void parseResponse(String resp){
        //test for json
        JsonObject json = parser.parse(MockData_View.testJson1).getAsJsonObject();
        if(json.get("title").getAsString().equals("Point")){
            System.out.println("Successful json parse. Have a point...");
            int x = json.get("properties").getAsJsonObject().get("x").getAsInt();
            int y = json.get("properties").getAsJsonObject().get("y").getAsInt();
            System.out.println("Point parsed: " + x + "," + y);
        }
        
    }
    */
    
    
    
    /*
    
    public static String FrameJson()
     {
          String obj = Json.createObjectBuilder()
            .add("title", "GetFrameParams")
            .add("type", "object")
            .add("properties", Json.createObjectBuilder()
                    .add("world_name",Json.createObjectBuilder()
                            .add("type","string")
                    )
                    .add("frame_id", Json.createObjectBuilder()
                            .add("type", "integer")))
             .add("required", Json.createArrayBuilder()
                     .add("frame_id")
                     .add("world_name")
             )
            .build()
            .toString();
              
                  
          return obj;
         
     }
     public static String NextParamJson()
     {
          String obj = Json.createObjectBuilder()
            .add("title", "GetFrameParams")
            .add("type", "object")
            .add("properties", Json.createObjectBuilder()
                    .add("world_name",Json.createObjectBuilder()
                            .add("type","string")
                    )
                    .add("frame_id", Json.createObjectBuilder()
                            .add("type", "integer")))
             .add("required", Json.createArrayBuilder()
                     .add("frame_id")
                     .add("world_name")
             )
            .build()
            .toString();
              
                  
          return obj;
         
     }
     
     public static String newFrameJson()
     {
          String obj = Json.createObjectBuilder()
            .add("title", "GetFrameParams")
            .add("type", "object")
            .add("properties", Json.createObjectBuilder()
                    .add("world_name",Json.createObjectBuilder()
                            .add("type","string")
                    )
                    .add("live_cells", Json.createObjectBuilder()
                            .add("description", "Any cells the new frame should start off with")
                            .add("type", "array")
                            .add("items",Json.createObjectBuilder()
                                .add("$ref","file:point.schema.json"))
                            .add("uniqueItems", true)
                            ))
             .add("required", Json.createArrayBuilder()
                     .add("frame_id")
                     .add("world_name")
             )
            .build()
            .toString();    
          return obj;
         
     }
    */
    
}
