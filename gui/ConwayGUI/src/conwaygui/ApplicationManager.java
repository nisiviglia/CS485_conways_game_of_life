
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package conwaygui;

import com.google.gson.Gson;
import com.google.gson.JsonArray;
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;
import java.awt.Color;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Timer;
import java.util.TimerTask;
import java.util.logging.Level;
import java.util.logging.Logger;


/**
 *
 * @author Steve Coluccio
 * 
 * 
 * ApplicationManager.java
 * 
 *  This is a global manager for information shared across the application.
 * 
 */
public class ApplicationManager {
    
    //Application settings
    public static int WINDOW_WIDTH_DEFAULT = 800;
    public static int WINDOW_HEIGHT_DEFAULT = 600;
    public static Color COLOR_LINE_DEFAULT = Color.WHITE;
    public static Color COLOR_BG_DEFAULT = Color.BLACK;
    public static Color COLOR_NORMAL_HIGHLIGHT = new Color(255, 255, 100, 128); //rgba
    public static Color COLOR_SELECTION_DRAG = new Color(100, 255, 255, 128); //rgba
    public static Color COLOR_SELECTION_HIGHLIGHT = new Color(100, 255, 255, 32); //rgba
    
    public static RenderPanel renderPanelCurrent;
    
    //Frame Render info
    public static String GRID_NAME_DEFAULT = "DefaultGrid";
    public static String currentGridName;
    public static int currentFrameId;
    public static final int CELL_WIDTH_BASE = 64; //px, cells are squares so no need for height
    public static final int CELL_WIDTH_MAX = 64;
    public static final int CELL_WIDTH_MIN = 16;
    public static float zoomCurrent = 1.0f; //100%
    public static int cellWidthCurrent = 64;
    //Default Mouse state on application launch
    public static MouseState mouseState = MouseState.POINTER;
    //coordinate groupings
    public static CoordGrouping currentGroup;
    //internal timer for play runtime sim
    private static Timer playTimer;
    public static int playTimerSpeed = 1000; //ms
    
    //json messaging
    public static CommandManager commandManager;
    public static JsonParser parser = new JsonParser();
    
    //KEY BINDINGS (key codes for keys pressed on GuiMain)
    public static final int KEY_UPARROW = 38;
    public static final int KEY_DOWNARROW = 40;
    public static final int KEY_RIGHTARROW = 39;
    public static final int KEY_LEFTARROW = 37;
    public static final int KEY_PAGEUP = 33;
    public static final int KEY_PAGEDOWN = 34;
    public static final int KEY_ENTER = 10;
    public static final int KEY_SPACE = 32;
    public static final int KEY_1 = 49;
    public static final int KEY_2 = 50;
    public static final int KEY_3 = 51;
    public static final int KEY_4 = 52;
    public static final int KEY_5 = 53;
    
    
    //Mouse state related to different tools functionality
    //There will alwyays be a mouse state
    public enum MouseState{
        POINTER,
        SELECTION,
        PAN,
        SIM_PLAY
    }
    
    public enum MoveDirection{
        UP,
        DOWN,
        LEFT,
        RIGHT,
        NONE
    }
    
    /****************************************************************************
    * Create a new Grid
    *   -current gui implmentation
     * @param gridName
    * @TODO: THis requires a message sent to @EXECUTION to create a new grid
    ****************************************************************************/
    public static void initRenderPanel(String gridName) throws FileNotFoundException, IOException{
        currentGridName = gridName;
        renderPanelCurrent.newGrid();
        setCellWidth();
        
        //alert exec of new world
        sendMessageNewWorld(gridName);
        
        //readMessage(MockData_View.createJsonString());
    }
    
    /****************************************************************************
     * Key Input from GuiMain
     *      -route input to functions here
     * 
     * @param keyCode - raw ascii keycode
    ****************************************************************************/
    
    public static void keyPress(int keyCode) throws FileNotFoundException, IOException{
        switch(keyCode){
            case KEY_UPARROW: 
                renderPanelCurrent.move(MoveDirection.UP);
                break;
            case KEY_DOWNARROW:
                renderPanelCurrent.move(MoveDirection.DOWN);
                break;
            case KEY_RIGHTARROW:
                renderPanelCurrent.move(MoveDirection.RIGHT);
                break;
            case KEY_LEFTARROW:
                renderPanelCurrent.move(MoveDirection.LEFT);
                break;
            case KEY_PAGEUP:
                menuZoom(true);
                break;
            case KEY_PAGEDOWN:
                menuZoom(false);
                break;
            case KEY_ENTER:
                stepForward();
                break;
            case KEY_SPACE:
                if(mouseState == MouseState.SIM_PLAY){
                    pause();
                }else{
                    play();
                }
                break;
            case KEY_1:
                setPlaySpeed(1);
                break;
            case KEY_2:
                setPlaySpeed(2);
                break;
            case KEY_3:
                setPlaySpeed(3);
                break;
            case KEY_4:
                setPlaySpeed(4);
                break;
            case KEY_5:
                setPlaySpeed(5);
                break;   
        }
    }
    
    /**************************************************************************
    @Description: Set the mouse state to one of the available MouseStates,
        related to the mouse tools. 
    
    NOTE: This method will allow for more flexibility when changing the mouse state.
        
    @param: state - new mouse state
    @return: 
        true - changed to new state
        false - state unchanged (already set to that state)
    ***************************************************************************/
        
    public static boolean setMouseState(MouseState state){
        if(state == mouseState) { return false; }
        mouseState = state;
        return true;
    }
    
    
    /****************************************************************************
            ZOOM
    
    *    @TODO: Link this up to the event listeners over in GuiMain
    ****************************************************************************/
    public static void menuZoom(boolean zoomIn){
        if(zoomIn){
            if(CELL_WIDTH_BASE * zoomCurrent >= CELL_WIDTH_MAX) {return;}
           zoomCurrent *= 1.1f; 
        }else{
            if(CELL_WIDTH_BASE * zoomCurrent <= CELL_WIDTH_MIN) {return;}
           zoomCurrent /= 1.1f;
        }
        setCellWidth();
        renderPanelCurrent.repaint();
    }
    
    public static void setCellWidth(){
        cellWidthCurrent = (int)(CELL_WIDTH_BASE * zoomCurrent);
    }
    
    public static void stepForward() throws FileNotFoundException, IOException{
        //grid has been editted
        //send new grid info to pipe
        //then read the response
        if(renderPanelCurrent.isDirty()){ 
            sendMessageNewFrame();
        }else{
            sendMessageNextFrame();
        }
        System.out.println("Frame step : Dirty = " + renderPanelCurrent.isDirty());
    }
    
    
    public static void play(){
        System.out.println("PLAYED");
        //disable editing while playing
        mouseState = MouseState.SIM_PLAY;
        
        //start timer
        playTimer = new Timer();
        setTimerSchedule();
    }
    
    
    public static void pause(){
        System.out.println("PAUSED");
        mouseState = MouseState.POINTER;
        
        if(playTimer != null) {
            playTimer.cancel();
        } 
    }
    
    
    private static void setTimerSchedule(){
        playTimer.schedule(new TimerTask(){
            @Override
            public void run() {
                System.out.println("TIMER EXECUTE w/ period of " + playTimerSpeed + "ms");
                try{
                    sendMessageNextFrame();
                } catch(FileNotFoundException ex){
                    System.err.println(ex);
                } catch (IOException ex) {
                    System.err.println(ex);
                }
            }
        }, 0, playTimerSpeed);
    }
    
    
    public static void setPlaySpeed(int index){
        switch(index){
            case 1:
                playTimerSpeed = 1000;
                break;
            case 2: 
                playTimerSpeed = 500;
                break;
            case 3:
                playTimerSpeed = 250;
                break;
            case 4:
                playTimerSpeed = 125;
                break;
            case 5:
                playTimerSpeed = 33; //30 fps
                break;
        }
        //clean up existing timer
        if(mouseState == MouseState.SIM_PLAY){
            pause();
            play();
            //setTimerSchedule();
        }
        System.out.println("Simulation Speed set to : " + playTimerSpeed + "ms");
    }
    
    
    public static void stepBackwards(){}
    public static void save(){}
    public static void load(){}
    public static void importGrid(){}
    public static void copySelection(){}
    public static void cutSelection(){}
    public static void pasteSelection(){}
    public static void rotateSelection(){}
    
    
    
    //JSON PARSING
    /****************************************************************************
     * Parse Json Messages coming from Comamnd Manager
     * 
     * @param jsonString - json string to parse
    ****************************************************************************/
    public static void readMessage(String jsonString){
        try{
            JsonObject json = parser.parse(jsonString).getAsJsonObject();
            if(json.get("success").getAsBoolean()){
                currentFrameId = json.get("frame_id").getAsInt();
                JsonArray live_cells = json.get("live_cells").getAsJsonArray();
                ArrayList<Coordinate> liveCellsForGrid = new ArrayList<Coordinate>();
                for(JsonElement point : live_cells){
                    int x = point.getAsJsonObject().get("x").getAsInt();
                    int y = point.getAsJsonObject().get("y").getAsInt();
                    System.out.print("[ " + x + "," + y + " ], ");
                    liveCellsForGrid.add(new Coordinate(x,y));
                }
                renderPanelCurrent.newGrid(liveCellsForGrid);
                renderPanelCurrent.repaint();
            }
        }catch(NullPointerException ex){
            System.err.println("NULL Response from execution pipe!");
        }
    }
    
    /*********************************************************************************************
     * Create Message of the entire frame
    ********************************************************************************************/
    public static void sendMessageNewFrame() throws FileNotFoundException, IOException{
        //JsonResult jsonObj = new JsonResult();
        JsonObject jsonObj = new JsonObject();
        jsonObj.addProperty("command", "new_frame");
        jsonObj.addProperty("return_loc", commandManager.GUI_OUT_FILE_NAME);

        JsonObject jsonParams = new JsonObject();
        jsonParams.addProperty("world_name", currentGridName);
        jsonParams.addProperty("frame_id", currentFrameId);
        
        //jsonObj.properties.live_cells = new JsonPoint[renderPanelCurrent.getLivingCells().size()];
        JsonArray cells = new JsonArray();
        for(int i = 0; i < renderPanelCurrent.getLivingCells().size(); i++){
            JsonObject jsonCell = new JsonObject();
            jsonCell.addProperty("x", renderPanelCurrent.getLivingCells().get(i).x);
            jsonCell.addProperty("y", renderPanelCurrent.getLivingCells().get(i).y);
            cells.add(jsonCell);
        }
        jsonParams.add("live_cells", cells);

        jsonObj.add("params", jsonParams);
        
        //send this json to the command manager pipe
        readMessage(commandManager.sendMessage(new Gson().toJson(jsonObj)));
    }
    
    
    public static void sendMessageNextFrame() throws FileNotFoundException, IOException{
        JsonObject jsonCommand = new JsonObject();
        jsonCommand.addProperty("command", "get_next");
        jsonCommand.addProperty("return_loc", commandManager.GUI_OUT_FILE_NAME);
        
        JsonObject jsonObj = new JsonObject();
        jsonObj.addProperty("world_name", currentGridName);
        jsonObj.addProperty("frame_id", currentFrameId);
        jsonCommand.add("params", jsonObj);
        
        //send this json over to the command manager and wait for a resp
        readMessage(commandManager.sendMessage(new Gson().toJson(jsonCommand)));
    }
    
    
    public static void sendMessageNewWorld(String worldName) throws FileNotFoundException, IOException{
        JsonObject jsonCommand = new JsonObject();
        jsonCommand.addProperty("command", "new_world");
        jsonCommand.addProperty("return_loc", commandManager.GUI_OUT_FILE_NAME);
        
        JsonObject jsonObj = new JsonObject();
        jsonObj.addProperty("world_name", worldName);
        jsonCommand.add("params", jsonObj);
        
        //send this json over to the command manager and wait for a resp
        readMessage(commandManager.sendMessage(new Gson().toJson(jsonCommand)));
    }   
}
