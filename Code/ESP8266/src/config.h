#define SSID "OrionC_1"         //Kako
#define PASS "Xr4q7PkW25**22"   //dmmq9361

static long gmtoffset_sec = 3600; 
static int daylightoffset_sec = 0;
static const char* ntpserver = "0.cz.pool.ntp.org";

int animation_interval_sec = random(60, 240);
u_int animation_next;

int ntp_update_interval_sec = 43200;
int ntp_update_interval_failed = 240;
u_int ntp_update_next;

int date_interval_sec;

