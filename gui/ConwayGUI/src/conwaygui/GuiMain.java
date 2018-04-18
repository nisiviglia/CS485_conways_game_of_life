/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package conwaygui;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author saynt
 */
public class GuiMain extends javax.swing.JFrame {

    /**
     * Creates new form GuiMain
     */
    public GuiMain() {
        initComponents();

        
        //set initial render panel
        ApplicationManager.renderPanelCurrent = renderPanel1;
    }

    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jPanel1 = new javax.swing.JPanel();
        renderPanel1 = new conwaygui.RenderPanel();
        mainMenuBar = new javax.swing.JMenuBar();
        menuOptionFile = new javax.swing.JMenu();
        menuItemNewGrid = new javax.swing.JMenuItem();
        menuItemSave = new javax.swing.JMenuItem();
        menuItemSaveAs = new javax.swing.JMenuItem();
        menuItemLoad = new javax.swing.JMenuItem();
        menuItemImport = new javax.swing.JMenuItem();
        menuItemClose = new javax.swing.JMenuItem();
        menuOptionEdit = new javax.swing.JMenu();
        menuItemPointer = new javax.swing.JMenuItem();
        menuItemSelectionTool = new javax.swing.JMenuItem();
        menuItemPan = new javax.swing.JMenuItem();
        menuItemCopy = new javax.swing.JMenuItem();
        menuItemCut = new javax.swing.JMenuItem();
        menuItemPaste = new javax.swing.JMenuItem();
        menuItemRotateCW = new javax.swing.JMenuItem();
        menuItemRotateCCW = new javax.swing.JMenuItem();
        menuItemZoomIn = new javax.swing.JMenuItem();
        menuItemZoomOut = new javax.swing.JMenuItem();
        menuOptionRuntime = new javax.swing.JMenu();
        menuItemPlay = new javax.swing.JMenuItem();
        menuItemPause = new javax.swing.JMenuItem();
        menuItemStepForward = new javax.swing.JMenuItem();
        menuItemStepBackwards = new javax.swing.JMenuItem();
        menuItemSpeed1 = new javax.swing.JMenuItem();
        menuItemSpeed2 = new javax.swing.JMenuItem();
        menuItemSpeed3 = new javax.swing.JMenuItem();
        menuItemSpeed4 = new javax.swing.JMenuItem();
        menuItemSpeed5 = new javax.swing.JMenuItem();
        menuOptionWindows = new javax.swing.JMenu();
        menuOptionHelp = new javax.swing.JMenu();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        addMouseWheelListener(new java.awt.event.MouseWheelListener() {
            public void mouseWheelMoved(java.awt.event.MouseWheelEvent evt) {
                formMouseWheelMoved(evt);
            }
        });
        addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                formKeyReleased(evt);
            }
        });

        javax.swing.GroupLayout jPanel1Layout = new javax.swing.GroupLayout(jPanel1);
        jPanel1.setLayout(jPanel1Layout);
        jPanel1Layout.setHorizontalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 690, Short.MAX_VALUE)
        );
        jPanel1Layout.setVerticalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 424, Short.MAX_VALUE)
        );

        renderPanel1.setEnabled(false);

        javax.swing.GroupLayout renderPanel1Layout = new javax.swing.GroupLayout(renderPanel1);
        renderPanel1.setLayout(renderPanel1Layout);
        renderPanel1Layout.setHorizontalGroup(
            renderPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 690, Short.MAX_VALUE)
        );
        renderPanel1Layout.setVerticalGroup(
            renderPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 424, Short.MAX_VALUE)
        );

        mainMenuBar.setToolTipText("MainMenuBar");

        menuOptionFile.setText("File");

        menuItemNewGrid.setText("New Grid");
        menuItemNewGrid.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuNewGridAction(evt);
            }
        });
        menuOptionFile.add(menuItemNewGrid);

        menuItemSave.setText("Save");
        menuItemSave.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuSaveAction(evt);
            }
        });
        menuOptionFile.add(menuItemSave);

        menuItemSaveAs.setText("Save As");
        menuItemSaveAs.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuItemSaveAsActionPerformed(evt);
            }
        });
        menuOptionFile.add(menuItemSaveAs);

        menuItemLoad.setText("Load");
        menuItemLoad.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuItemLoadActionPerformed(evt);
            }
        });
        menuOptionFile.add(menuItemLoad);

        menuItemImport.setText("Import");
        menuItemImport.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuItemImportActionPerformed(evt);
            }
        });
        menuOptionFile.add(menuItemImport);

        menuItemClose.setText("Close");
        menuItemClose.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuItemCloseActionPerformed(evt);
            }
        });
        menuOptionFile.add(menuItemClose);

        mainMenuBar.add(menuOptionFile);

        menuOptionEdit.setText("Edit");

        menuItemPointer.setText("Pointer Tool");
        menuItemPointer.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuItemPointerActionPerformed(evt);
            }
        });
        menuOptionEdit.add(menuItemPointer);

        menuItemSelectionTool.setText("Selection Tool");
        menuItemSelectionTool.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuItemSelectionToolActionPerformed(evt);
            }
        });
        menuOptionEdit.add(menuItemSelectionTool);

        menuItemPan.setText("Pan Tool");
        menuItemPan.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuItemPanActionPerformed(evt);
            }
        });
        menuOptionEdit.add(menuItemPan);

        menuItemCopy.setText("Copy");
        menuItemCopy.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuItemCopyActionPerformed(evt);
            }
        });
        menuOptionEdit.add(menuItemCopy);

        menuItemCut.setText("Cut");
        menuItemCut.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuItemCutActionPerformed(evt);
            }
        });
        menuOptionEdit.add(menuItemCut);

        menuItemPaste.setText("Paste");
        menuItemPaste.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuItemPasteActionPerformed(evt);
            }
        });
        menuOptionEdit.add(menuItemPaste);

        menuItemRotateCW.setText("Rotate CW");
        menuItemRotateCW.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuItemRotateCWActionPerformed(evt);
            }
        });
        menuOptionEdit.add(menuItemRotateCW);

        menuItemRotateCCW.setText("Rotate CCW");
        menuItemRotateCCW.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuItemRotateCCWActionPerformed(evt);
            }
        });
        menuOptionEdit.add(menuItemRotateCCW);

        menuItemZoomIn.setText("Zoom In");
        menuItemZoomIn.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuItemZoomInActionPerformed(evt);
            }
        });
        menuOptionEdit.add(menuItemZoomIn);

        menuItemZoomOut.setText("Zoom Out");
        menuItemZoomOut.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuItemZoomOutActionPerformed(evt);
            }
        });
        menuOptionEdit.add(menuItemZoomOut);

        mainMenuBar.add(menuOptionEdit);

        menuOptionRuntime.setText("Runtime");

        menuItemPlay.setText("Play");
        menuItemPlay.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuItemPlayActionPerformed(evt);
            }
        });
        menuOptionRuntime.add(menuItemPlay);

        menuItemPause.setText("Pause");
        menuItemPause.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuItemPauseActionPerformed(evt);
            }
        });
        menuOptionRuntime.add(menuItemPause);

        menuItemStepForward.setText("Step Forward");
        menuItemStepForward.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuItemStepForwardActionPerformed(evt);
            }
        });
        menuOptionRuntime.add(menuItemStepForward);

        menuItemStepBackwards.setText("Step Backwards");
        menuItemStepBackwards.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuItemStepBackwardsActionPerformed(evt);
            }
        });
        menuOptionRuntime.add(menuItemStepBackwards);

        menuItemSpeed1.setText("Speed (1 second)");
        menuItemSpeed1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuItemSpeed1ActionPerformed(evt);
            }
        });
        menuOptionRuntime.add(menuItemSpeed1);
        menuItemSpeed1.getAccessibleContext().setAccessibleDescription("");

        menuItemSpeed2.setText("Speed (500 ms)");
        menuItemSpeed2.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuItemSpeed2ActionPerformed(evt);
            }
        });
        menuOptionRuntime.add(menuItemSpeed2);

        menuItemSpeed3.setText("Speed (250 ms)");
        menuItemSpeed3.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuItemSpeed3ActionPerformed(evt);
            }
        });
        menuOptionRuntime.add(menuItemSpeed3);

        menuItemSpeed4.setText("Speed (125 ms)");
        menuItemSpeed4.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuItemSpeed4ActionPerformed(evt);
            }
        });
        menuOptionRuntime.add(menuItemSpeed4);

        menuItemSpeed5.setText("Speed (33 ms)");
        menuItemSpeed5.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuItemSpeed5ActionPerformed(evt);
            }
        });
        menuOptionRuntime.add(menuItemSpeed5);

        mainMenuBar.add(menuOptionRuntime);

        menuOptionWindows.setText("Windows");
        mainMenuBar.add(menuOptionWindows);

        menuOptionHelp.setText("Help");
        mainMenuBar.add(menuOptionHelp);

        setJMenuBar(mainMenuBar);
        mainMenuBar.getAccessibleContext().setAccessibleName("MainMenuBar");

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 690, Short.MAX_VALUE)
            .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                .addComponent(jPanel1, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
            .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                .addGroup(layout.createSequentialGroup()
                    .addGap(0, 0, Short.MAX_VALUE)
                    .addComponent(renderPanel1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addGap(0, 0, Short.MAX_VALUE)))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 424, Short.MAX_VALUE)
            .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                .addComponent(jPanel1, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
            .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                .addGroup(layout.createSequentialGroup()
                    .addGap(0, 0, Short.MAX_VALUE)
                    .addComponent(renderPanel1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addGap(0, 0, Short.MAX_VALUE)))
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    
    
    
    /****************************
    
        MENU FUNCTION ACTIONS
    
    ****************************/
    
    private void menuSaveAction(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuSaveAction
        // TODO add your handling code here:
        System.out.println(evt.getActionCommand() +  " called");
    }//GEN-LAST:event_menuSaveAction

    private void menuNewGridAction(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuNewGridAction
        // TODO add your handling code here:
        System.out.println(evt.getActionCommand() +  " called");
        new GuiFileSelection().setVisible(true);
        //ApplicationManager.InitRenderPanel(renderPanel1); 
        repaint();
    }//GEN-LAST:event_menuNewGridAction

    private void menuItemSaveAsActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuItemSaveAsActionPerformed
        // TODO add your handling code here:
        System.out.println(evt.getActionCommand() +  " called");
        ApplicationManager.save();
    }//GEN-LAST:event_menuItemSaveAsActionPerformed

    private void menuItemLoadActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuItemLoadActionPerformed
        // TODO add your handling code here:
        System.out.println(evt.getActionCommand() +  " called");
        ApplicationManager.load();
    }//GEN-LAST:event_menuItemLoadActionPerformed

    private void menuItemImportActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuItemImportActionPerformed
        // TODO add your handling code here:
        System.out.println(evt.getActionCommand() +  " called");
        ApplicationManager.importGrid();
    }//GEN-LAST:event_menuItemImportActionPerformed

    private void menuItemCloseActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuItemCloseActionPerformed

        System.out.println(evt.getActionCommand() +  " called");
        dispatchEvent(new java.awt.event.WindowEvent(this, java.awt.event.WindowEvent.WINDOW_CLOSING));
    }//GEN-LAST:event_menuItemCloseActionPerformed

    private void menuItemSelectionToolActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuItemSelectionToolActionPerformed
        // TODO add your handling code here:
        System.out.println(evt.getActionCommand() +  " called");
        ApplicationManager.setMouseState(ApplicationManager.MouseState.SELECTION);
    }//GEN-LAST:event_menuItemSelectionToolActionPerformed

    private void menuItemCopyActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuItemCopyActionPerformed
        // TODO add your handling code here:
        System.out.println(evt.getActionCommand() +  " called");
        ApplicationManager.copySelection();
    }//GEN-LAST:event_menuItemCopyActionPerformed

    private void menuItemCutActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuItemCutActionPerformed
        // TODO add your handling code here:
        System.out.println(evt.getActionCommand() +  " called");
        ApplicationManager.cutSelection();
    }//GEN-LAST:event_menuItemCutActionPerformed

    private void menuItemPasteActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuItemPasteActionPerformed
        // TODO add your handling code here:
        System.out.println(evt.getActionCommand() +  " called");
        ApplicationManager.pasteSelection();
    }//GEN-LAST:event_menuItemPasteActionPerformed

    private void menuItemRotateCWActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuItemRotateCWActionPerformed
        // TODO add your handling code here:
        System.out.println(evt.getActionCommand() +  " called");
        //@TODO: Specify CW in parameter
        ApplicationManager.rotateSelection();
    }//GEN-LAST:event_menuItemRotateCWActionPerformed

    private void menuItemRotateCCWActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuItemRotateCCWActionPerformed
        // TODO add your handling code here:
        System.out.println(evt.getActionCommand() +  " called");
        //@TODO: specify CCW in parameter
        ApplicationManager.rotateSelection();
    }//GEN-LAST:event_menuItemRotateCCWActionPerformed

    private void menuItemZoomInActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuItemZoomInActionPerformed
        ApplicationManager.menuZoom(true);
        repaint();
    }//GEN-LAST:event_menuItemZoomInActionPerformed

    private void menuItemZoomOutActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuItemZoomOutActionPerformed
        ApplicationManager.menuZoom(false);
        repaint();
    }//GEN-LAST:event_menuItemZoomOutActionPerformed

    private void menuItemPlayActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuItemPlayActionPerformed
        // TODO add your handling code here:
        System.out.println(evt.getActionCommand() +  " called");
        ApplicationManager.play();
    }//GEN-LAST:event_menuItemPlayActionPerformed

    private void menuItemPauseActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuItemPauseActionPerformed
        // TODO add your handling code here:
        System.out.println(evt.getActionCommand() +  " called");
        ApplicationManager.pause();
    }//GEN-LAST:event_menuItemPauseActionPerformed

    private void menuItemStepForwardActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuItemStepForwardActionPerformed
        // TODO add your handling code here:
        System.out.println(evt.getActionCommand() +  " called");
        try {
            ApplicationManager.stepForward();
        } catch (FileNotFoundException ex) {
            Logger.getLogger(GuiMain.class.getName()).log(Level.SEVERE, null, ex);
        } catch (IOException ex) {
            Logger.getLogger(GuiMain.class.getName()).log(Level.SEVERE, null, ex);
        }
    }//GEN-LAST:event_menuItemStepForwardActionPerformed

    private void menuItemStepBackwardsActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuItemStepBackwardsActionPerformed
        // TODO add your handling code here:
        System.out.println(evt.getActionCommand() +  " called");
        ApplicationManager.stepBackwards();
    }//GEN-LAST:event_menuItemStepBackwardsActionPerformed

    private void menuItemPanActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuItemPanActionPerformed
        // TODO add your handling code here:
        System.out.println(evt.getActionCommand() +  " called");
        ApplicationManager.setMouseState(ApplicationManager.MouseState.PAN);
    }//GEN-LAST:event_menuItemPanActionPerformed

    private void menuItemPointerActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuItemPointerActionPerformed
        System.out.println(evt.getActionCommand() +  " called");
        ApplicationManager.setMouseState(ApplicationManager.MouseState.POINTER);
    }//GEN-LAST:event_menuItemPointerActionPerformed

    private void formKeyReleased(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_formKeyReleased
        System.out.println("KeyCode Pressed: " + evt.getKeyCode());
        try {
            ApplicationManager.keyPress(evt.getKeyCode());
        } catch (FileNotFoundException ex) {
            Logger.getLogger(GuiMain.class.getName()).log(Level.SEVERE, null, ex);
        } catch (IOException ex) {
            Logger.getLogger(GuiMain.class.getName()).log(Level.SEVERE, null, ex);
        }
    }//GEN-LAST:event_formKeyReleased

    private void formMouseWheelMoved(java.awt.event.MouseWheelEvent evt) {//GEN-FIRST:event_formMouseWheelMoved
        System.out.println(evt.getWheelRotation());
        ApplicationManager.menuZoom(evt.getWheelRotation() < 0);
    }//GEN-LAST:event_formMouseWheelMoved

    private void menuItemSpeed1ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuItemSpeed1ActionPerformed
        System.out.println(evt.getActionCommand() +  " called");
        ApplicationManager.setPlaySpeed(1);
    }//GEN-LAST:event_menuItemSpeed1ActionPerformed

    private void menuItemSpeed2ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuItemSpeed2ActionPerformed
        System.out.println(evt.getActionCommand() +  " called");
        ApplicationManager.setPlaySpeed(2);
    }//GEN-LAST:event_menuItemSpeed2ActionPerformed

    private void menuItemSpeed3ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuItemSpeed3ActionPerformed
        System.out.println(evt.getActionCommand() +  " called");
        ApplicationManager.setPlaySpeed(3);
    }//GEN-LAST:event_menuItemSpeed3ActionPerformed

    private void menuItemSpeed4ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuItemSpeed4ActionPerformed
        System.out.println(evt.getActionCommand() +  " called");
        ApplicationManager.setPlaySpeed(4);
    }//GEN-LAST:event_menuItemSpeed4ActionPerformed

    private void menuItemSpeed5ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuItemSpeed5ActionPerformed
        System.out.println(evt.getActionCommand() +  " called");
        ApplicationManager.setPlaySpeed(5);
    }//GEN-LAST:event_menuItemSpeed5ActionPerformed

    /**
     * @param args the command line arguments
     */
    public static void main(String args[]) {
        /* Set the Nimbus look and feel */
        //<editor-fold defaultstate="collapsed" desc=" Look and feel setting code (optional) ">
        /* If Nimbus (introduced in Java SE 6) is not available, stay with the default look and feel.
         * For details see http://download.oracle.com/javase/tutorial/uiswing/lookandfeel/plaf.html 
         */
        try {
            for (javax.swing.UIManager.LookAndFeelInfo info : javax.swing.UIManager.getInstalledLookAndFeels()) {
                if ("Nimbus".equals(info.getName())) {
                    javax.swing.UIManager.setLookAndFeel(info.getClassName());
                    break;
                }
            }
        } catch (ClassNotFoundException ex) {
            java.util.logging.Logger.getLogger(GuiMain.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(GuiMain.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(GuiMain.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(GuiMain.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>
        
        CommandManager commandManager;
        if(args.length == 0){
            System.out.println("No args found. Creating a dummy CommandManager");
            commandManager = new CommandManager("fake", "fake", "fake");
        }else{
            commandManager = new CommandManager(args[0], args[1], args[2]);
            commandManager.createPipe();
        }
        ApplicationManager.commandManager = commandManager;
        
        /* Create and display the form */
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                OnCloseListener closeListener = new OnCloseListener(commandManager);  

                GuiMain main = new GuiMain();
                main.addWindowListener(closeListener);
                main.setVisible(true);
                
                //Initial frame for grid setup
                GuiFileSelection fileSelection = new GuiFileSelection();
                fileSelection.setVisible(true);
            }
        });
    }

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JPanel jPanel1;
    private javax.swing.JMenuBar mainMenuBar;
    private javax.swing.JMenuItem menuItemClose;
    private javax.swing.JMenuItem menuItemCopy;
    private javax.swing.JMenuItem menuItemCut;
    private javax.swing.JMenuItem menuItemImport;
    private javax.swing.JMenuItem menuItemLoad;
    private javax.swing.JMenuItem menuItemNewGrid;
    private javax.swing.JMenuItem menuItemPan;
    private javax.swing.JMenuItem menuItemPaste;
    private javax.swing.JMenuItem menuItemPause;
    private javax.swing.JMenuItem menuItemPlay;
    private javax.swing.JMenuItem menuItemPointer;
    private javax.swing.JMenuItem menuItemRotateCCW;
    private javax.swing.JMenuItem menuItemRotateCW;
    private javax.swing.JMenuItem menuItemSave;
    private javax.swing.JMenuItem menuItemSaveAs;
    private javax.swing.JMenuItem menuItemSelectionTool;
    private javax.swing.JMenuItem menuItemSpeed1;
    private javax.swing.JMenuItem menuItemSpeed2;
    private javax.swing.JMenuItem menuItemSpeed3;
    private javax.swing.JMenuItem menuItemSpeed4;
    private javax.swing.JMenuItem menuItemSpeed5;
    private javax.swing.JMenuItem menuItemStepBackwards;
    private javax.swing.JMenuItem menuItemStepForward;
    private javax.swing.JMenuItem menuItemZoomIn;
    private javax.swing.JMenuItem menuItemZoomOut;
    private javax.swing.JMenu menuOptionEdit;
    private javax.swing.JMenu menuOptionFile;
    private javax.swing.JMenu menuOptionHelp;
    private javax.swing.JMenu menuOptionRuntime;
    private javax.swing.JMenu menuOptionWindows;
    private conwaygui.RenderPanel renderPanel1;
    // End of variables declaration//GEN-END:variables
}
