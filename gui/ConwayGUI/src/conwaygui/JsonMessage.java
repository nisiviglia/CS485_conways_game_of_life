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
public class JsonMessage {
    
    public static class JsonResult{
        public String title = "Result";
        public JsonProperties properties = new JsonProperties();
    }
  
    public static class JsonProperties{
        public String world_name;
        public int frame_id;
        public JsonPoint[] live_cells;
    }
    
    public static class JsonPoint{
        public PointProperties properties;
        
        public JsonPoint(int x, int y){
            properties = new PointProperties();
            properties.x = x;
            properties.y = y;
        }
        
        private class PointProperties{
            public int x;
            public int y;
        }
    }
    
}
