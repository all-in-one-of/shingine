class CSceneManager;

class CStatics
{
public:
    static CSceneManager* GetSceneManager();
    
private:
    CStatics();
    static CSceneManager* SceneManager;
};
