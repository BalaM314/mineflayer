#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <OGRE/OgreCamera.h>
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreLogManager.h>
#include <OGRE/OgreRoot.h>
#include <OGRE/OgreViewport.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreConfigFile.h>
#include <OGRE/OgreVector3.h>

#include <OIS/OISEvents.h>
#include <OIS/OISInputManager.h>
#include <OIS/OISKeyboard.h>
#include <OIS/OISMouse.h>

#include <OGRE/SdkTrays.h>
#include <OGRE/SdkCameraMan.h>

#include <QObject>
#include <QHash>
#include <QSharedPointer>

#include "Chunk.h"
#include "Server.h"

class MainWindow :
    public QObject,
    public Ogre::FrameListener,
    public Ogre::WindowEventListener,
    public OIS::KeyListener,
    public OIS::MouseListener,
    OgreBites::SdkTrayListener
{
    Q_OBJECT
public:
    enum Control {
        Forward,
        Back,
        Left,
        Right,
        Jump,
        Crouch,
        DiscardItem,
        Action1, // left click
        Action2, // right click
        Inventory,
        Chat,
    };

public:
    MainWindow();
    virtual ~MainWindow();

    virtual void go();

protected:
    virtual bool setup();
    virtual bool configure();
    virtual void chooseSceneManager();
    virtual void createCamera();
    virtual void createFrameListener();
    virtual void createScene();
    virtual void destroyScene();
    virtual void createViewports();
    virtual void setupResources();
    virtual void createResourceListener();
    virtual void loadResources();

    // Ogre::FrameListener
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    // OIS::KeyListener
    virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg );
    // OIS::MouseListener
    virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

    // Ogre::WindowEventListener
    //Adjust mouse clipping area
    virtual void windowResized(Ogre::RenderWindow* rw);
    //Unattach OIS before window shutdown (very important under Linux)
    virtual void windowClosed(Ogre::RenderWindow* rw);

    Ogre::Root *m_root;
    Ogre::Camera* m_camera;
    Ogre::SceneManager* m_scene_manager;
    Ogre::RenderWindow* m_window;
    Ogre::String m_resources_config;
    Ogre::String m_plugins_config;

    // OgreBites
    OgreBites::SdkTrayManager* m_tray_manager;
    OgreBites::SdkCameraMan* m_camera_man;       // basic camera controller
    OgreBites::ParamsPanel* m_details_panel;     // sample details panel
    bool m_cursor_was_visible;                    // was cursor visible before dialog appeared
    bool m_shut_down;

    //OIS Input devices
    OIS::InputManager* m_input_manager;
    OIS::Mouse* m_mouse;
    OIS::Keyboard* m_keyboard;


private:

    struct Entity {
        Ogre::Vector3 pos;
        Ogre::Vector3 up;
        Ogre::Vector3 look;
        double stance;
        bool on_ground;
    };

    Server * m_server;

    QHash<Chunk::Coord, QSharedPointer<Chunk> > m_chunks;
    QHash<qint32, QSharedPointer<Entity> > m_entities;
    Entity * m_player;

    // maps OIS::KeyCode to Control and vice versa
    QHash<OIS::KeyCode, Control> m_key_to_control;
    QHash<Control, OIS::KeyCode> m_control_to_key;

    static const int c_fps;
    static const double c_time_per_frame_msecs;
    double m_target_time_msecs;


private:
    void loadControls();

};

#endif // MAINWINDOW_H