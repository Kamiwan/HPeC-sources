# This message contains useful arguments for the following nav orders:
#   LAND, TAKEOFF, GPS_MOVE, VELOCITY_MOVE, OBS_AVOIDANCE_MOVE, TRACKING_MOVE
#
# Parameters use case:
#   order           ->  used for all orders
#   altitude        ->  TAKEOFF, GPS_MOVE
#   latitude        ->  GPS_MOVE
#   longitude       ->  GPS_MOVE
#   yaw             ->  GPS_MOVE
#   vel_linear_x    ->  VELOCITY_MOVE  
#   vel_linear_y    ->  VELOCITY_MOVE
#   vel_linear_z    ->  VELOCITY_MOVE
#   distance        ->  VELOCITY_MOVE
#

string  order
float64 altitude
float64 latitude
float64 longitude
float64 yaw
int32   vel_linear_x
int32   vel_linear_y
int32   vel_linear_z
int32   distance 