/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package conwaygui;

import java.io.*;
import java.nio.charset.Charset;

/**
 *
 * @author Abanob Mikaeel
 */

public class CommandManager {
    String GUI_IN_FILE_NAME, GUI_OUT_FILE_NAME, PROCESS_PATH; 
    /// Definitions:
    
    // Connect to the named pipe
    File gui_to_exec_file;
    File exec_to_gui_file;

    // Reading from the pipe
    FileOutputStream gui_to_exec_stream; 
    FileInputStream exec_to_gui_stream;
    BufferedReader reader; 
             
             
    public CommandManager(String FirstArg, String SecondArg, String ThirdArg){
        PROCESS_PATH = FirstArg;
        GUI_OUT_FILE_NAME = SecondArg;
        GUI_IN_FILE_NAME = ThirdArg;
        
        gui_to_exec_file = new File(GUI_IN_FILE_NAME);
        exec_to_gui_file = new File(GUI_OUT_FILE_NAME);
    }
    
    public void createPipe() {
        try {
            String OpenPipe =  "mkfifo " + GUI_IN_FILE_NAME;
            Process openPipeProc = Runtime.getRuntime().exec(OpenPipe);
            
            String ReadingPipe =  "mkfifo " + GUI_OUT_FILE_NAME ;
            Process readPipe = Runtime.getRuntime().exec(ReadingPipe);

            printProcess(openPipeProc);
            printProcess(readPipe);
                        
            String command =  PROCESS_PATH + " " + GUI_IN_FILE_NAME;
            Runtime.getRuntime().exec(command);
        }
        catch (IOException e) {
            System.out.println("ran into an issue, "+e);
        }
    }
    
    public String sendMessage(String message) throws FileNotFoundException, IOException {
        System.out.println("The message is: "+message);
        String response, responseMessage = "" ;         
                
        try {     
            // Writing to the pipe
            System.out.println("Making output Stream");
            gui_to_exec_stream = new FileOutputStream(gui_to_exec_file);

            System.out.println("Writing Command");
            gui_to_exec_stream.write(message.getBytes(Charset.forName("UTF-8")));

            System.out.println("Command Written");
            
            System.out.println("Making Input Stream");
            exec_to_gui_stream = new FileInputStream(exec_to_gui_file);
            reader = new BufferedReader(new InputStreamReader(exec_to_gui_stream));
            while ( (response = reader.readLine()) != null ) {
                responseMessage += response;
            }  
        }
        catch (IOException ex) {
            System.out.println("Couldn't write to file "+ ex);
        } finally {  
            System.out.println("The length is +: " + responseMessage.length()+ "The response is: " + responseMessage);
            gui_to_exec_stream.close();
            reader.close();
            exec_to_gui_stream.close();
            
        }
        return responseMessage;
    }

    public void sendQuit() throws FileNotFoundException, IOException {
      System.out.println("Sending Quit");

      try {
        gui_to_exec_stream = new FileOutputStream(gui_to_exec_file);

        gui_to_exec_stream.write("{\"command\": \"quit\"}".getBytes(Charset.forName("UTF-8")));
      } catch (IOException ex) {
        System.out.println("Error Quitting :" + ex);
      } finally {
        gui_to_exec_stream.close();
      }
    }
    
    public void printProcess(Process proc) throws IOException {
    
            BufferedReader stdInput = new BufferedReader(new 
            InputStreamReader(proc.getInputStream()));

            BufferedReader stdError = new BufferedReader(new 
            InputStreamReader(proc.getErrorStream()));

            // read the output from the command
            System.out.println("Here is the standard output of the command:\n");
            String s = null;
            while ((s = stdInput.readLine()) != null) {
                System.out.println(s);
            }

            // read any errors from the attempted command
            System.out.println("Here is the standard error of the command (if any):\n");
            while ((s = stdError.readLine()) != null) {
                System.out.println(s);
            }
    }
}
