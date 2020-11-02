uint8_t stateMachine(uint8_t state)
{

  switch (atoi(state)){
  case 1:
    state = 2;
    break;
  case 2:
    state = 80;
    break;
  case 80:
    state = 81;
    break;
  case 81:
    state = 82;
    break;
  }

  return state;
}
