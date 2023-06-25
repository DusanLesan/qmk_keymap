// tap dance
typedef struct {
  bool is_press_action;
  int state;
} tap;

enum {
  SINGLE_TAP = 1,
  SINGLE_HOLD = 2,
  DOUBLE_TAP = 3
};

enum {
  QUOT_LAYR = 0
};

int cur_dance (tap_dance_state_t *state);
void ql_finished (tap_dance_state_t *state, void *user_data);
void ql_reset (tap_dance_state_t *state, void *user_data);

/Determine the current tap dance state
int cur_dance (tap_dance_state_t *state) {
  if (state->count == 1) {
    if (!state->pressed) {
      return SINGLE_TAP;
    } else {
      return SINGLE_HOLD;
    }
  } else if (state->count == 2) {
    return DOUBLE_TAP;
  }
  else return 8;
}

//Initialize tap structure associated with example tap dance key
static tap ql_tap_state = {
  .is_press_action = true,
  .state = 0
};

//Functions that control what our tap dance key does
void ql_finished (tap_dance_state_t *state, void *user_data) {
  ql_tap_state.state = cur_dance(state);
  switch (ql_tap_state.state) {
    case SINGLE_TAP:
      tap_code(KC_QUOT);
      break;
    case SINGLE_HOLD:
      layer_on(_NAV);
      break;
    case DOUBLE_TAP:
      //check to see if the layer is already set
      if (layer_state_is(_NAV)) {
        //if already set, then switch it off
        layer_off(_NAV);
      } else { 
        //if not already set, then switch the layer on
        layer_on(_NAV);
      }
      break;
  }
}

void ql_reset (tap_dance_state_t *state, void *user_data) {
  //if the key was held down and now is released then switch off the layer
  if (ql_tap_state.state==SINGLE_HOLD) {
    layer_off(_NAV);
  }
  ql_tap_state.state = 0;
}

//Associate our tap dance key with its functionality
tap_dance_action_t tap_dance_actions[] = {
  [QUOT_LAYR] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, ql_finished, ql_reset)
};
