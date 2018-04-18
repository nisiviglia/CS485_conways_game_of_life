package conwaygui;

import java.io.IOException;

public class OnCloseListener extends java.awt.event.WindowAdapter {
  public CommandManager commandManager;

  public OnCloseListener(CommandManager commandManager) {
    this.commandManager = commandManager;
  }

  @Override
  public void windowClosing(java.awt.event.WindowEvent windowEvent) {
    try {
      commandManager.sendQuit();
    } catch (IOException e) {
      System.out.println("Error occured:" + e);
    }

    System.out.println("Close Event");
    System.exit(0);
  }
}
