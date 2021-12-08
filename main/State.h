namespace BaseEngine {
  class BState {
    public:
      State();
      virtual void UpdateState() = 0;
      virtual void OnEntry() = 0;
      virtual void OnExit() = 0;
  };
}
