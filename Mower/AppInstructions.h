

class AppInstructions
{
  public:

    static AppInstructions* getInstance()
    {
      static AppInstructions* _instance = nullptr;
      if (_instance == nullptr)
      {
        _instance = new AppInstructions();
      }
      return _instance;
    }

    ~AppInstructions();
    
    int getVelocity();
    void setVelocity(int velocity);

    int getTurn();
    void setTurn(int turn);

    int getLight();
    void setLight(int light);

    int getHonk();
    void setHonk(int honk);

    int getAutoDrive();
    void setAutoDrive(int autodrive);

    void setInstructions(int velocity, int turn, int light, int honk, int autodrive);

    void setInstructionsAvailable(bool state);
    bool getInstructionsAvailable();

  private:
  
    AppInstructions();

    int _velocity = 0;
    int _turn = 0;
    int _light = 0;
    int _honk = 0;
    int _autoDrive = 0;
    bool _instructionsAvailable = false;
    
};
