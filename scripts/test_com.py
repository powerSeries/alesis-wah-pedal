# Counter loop function
import serial

guitar = RPR_GetTrack(0, 0)
count = RPR_TrackFX_GetCount(guitar)
fx1_state = False
fx2_state = False

def UpdateTrackState():
  global fx1_state, fx2_state
  fx1_state = RPR_TrackFX_GetEnabled(guitar, 0)
  fx2_state = RPR_TrackFX_GetEnabled(guitar, 1)

if count == 0:
  fx_1 = RPR_TrackFX_AddByName(guitar, "ReaComp (Cockos)", 0, -1)
  (_, guitar, _, fx_name, _) = RPR_TrackFX_GetFXName(guitar, 0, "", 512)
  if fx_1 >= 0:
    RPR_TrackFX_SetNamedConfigParm(guitar, 0, "renamed_name", "Guitar-1")
    
  
  
  fx_2 = RPR_TrackFX_AddByName(guitar, "ReaTune (Cockos)", 0, -1)
  (_, guitar, _, fx2_name, _) = RPR_TrackFX_GetFXName(guitar, 1, "", 512)
  if fx_2 >= 0:
    RPR_TrackFX_SetNamedConfigParm(guitar, 1, "renamed_name", "Guitar-2")
    RPR_TrackFX_SetEnabled(guitar, fx_2, 0)
    
  UpdateTrackState()
    
  
  
def switchFX():
  global guitar, fx1_state, fx2_state
  if fx1_state:
    RPR_TrackFX_SetEnabled(guitar, fx_1, 0)
    RPR_TrackFX_SetEnabled(guitar, fx_2, 1)
    
  if fx2_state:
    RPR_TrackFX_SetEnabled(guitar, fx_2, 0)
    RPR_TrackFX_SetEnabled(guitar, fx_1, 1)
    
  fx1_state = not fx1_state
  fx2_state = not fx1_state
  
  
#------------------- Reading Pedal Data
port = "COM3"
baud = 9600
ser = serial.Serial(port=port, baudrate=baud, timeout=1)

def counter_loop():
  global ser
  if ser is not None and ser.is_open:
    data = ser.readline().decode('utf-8').strip()
    
    if data == "Event!":
      switchFX()
      
  
  RPR_runloop("counter_loop()")

#------------------- Closing Script
def on_exit():
  global ser
  if ser.is_open:
    ser.close()
    
RPR_atexit("on_exit")
#------------------- Closing Script
counter_loop()
