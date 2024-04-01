#define SSID "ssid"
#define PASS "pass"

static long gmtoffset_sec = 3600;                       //timezone info
static int daylightoffset_sec = 3600;                   //summer winter time setting
static const char* ntpserver = "0.cz.pool.ntp.org";     //what NTP server to use, redirect to your local NTP pool (https://www.ntppool.org/)

int animation_interval_sec = random(60, 240);
u_int animation_next;

int ntp_update_interval_sec = 43200;
int ntp_update_interval_failed = 240;
u_int ntp_update_next;

int date_interval_sec;

