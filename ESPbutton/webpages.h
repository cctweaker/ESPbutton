const char notfound[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
  <title>ESPbutton</title>
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=2.0, minimum-scale=0.10">
  <meta name='mobile-web-app-capable' content='yes'>
  <meta http-equiv="refresh" content="3; url=/">
  <meta name='theme-color' content='#000'>
  <link rel='icon' sizes='256x256' href='https://esp.3dstar.ro/favicon.png'>
</head>
<body style="background-color:#000;color:#fff;text-align:center;">
    <h2>ESPbutton</h2>
    <br><br>
    <h3>Page not found<br><br>Click button or wait for page reload.</h3>
    <form action="/">
        <input type="submit" value="Main menu">
    </form>
    <br>
    <hr><br>
    <a href="https://3dstar.ro/">3DStar</a> -
    <a href="https://3dstar.ro/proiecte/espbutton">ESPbuton</a> -
    <a href="https://github.com/cctweaker/ESPbutton">GitHub</a>
    <br><br><br>
</body>
</html>
)=====";

const char restart[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
  <title>ESPbutton</title>
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=2.0, minimum-scale=0.10">
  <meta name='mobile-web-app-capable' content='yes'>
  <meta http-equiv="refresh" content="10; url=/">
  <meta name='theme-color' content='#000'>
  <link rel='icon' sizes='256x256' href='https://esp.3dstar.ro/favicon.png'>
</head>
<body style="background-color:#000;color:#fff;text-align:center;">
    <h2>ESPbutton<br>restart</h2>
    <br><br>
    <h3> Click button or wait for auto reload...</h3>
    <form action="/">
        <input type="submit" value="Main menu">
    </form>
    <br>
    <hr><br>
    <a href="https://3dstar.ro/">3DStar</a> -
    <a href="https://3dstar.ro/proiecte/espbutton">ESPbuton</a> -
    <a href="https://github.com/cctweaker/ESPbutton">GitHub</a>
    <br><br><br>
</body>
</html>
)=====";

const char format[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
  <title>ESPbutton</title>
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=2.0, minimum-scale=0.10">
  <meta name='mobile-web-app-capable' content='yes'>
  <meta name='theme-color' content='#000'>
  <link rel='icon' sizes='256x256' href='https://esp.3dstar.ro/favicon.png'>
</head>
<body style="background-color:#000;color:#fff;text-align:center;">
    <h2>ESPbutton<br><br><br>Settings erased!</h2>
    <br>
    <h3>Connect to wifi: "ESPbutton"<br>with password "abcd1234"<br>to begin the setup.</h3>
    <br><br><br><br><br>
    <hr><br>
    <a href="https://3dstar.ro/">3DStar</a> -
    <a href="https://3dstar.ro/proiecte/espbutton">ESPbuton</a> -
    <a href="https://github.com/cctweaker/ESPbutton">GitHub</a>
    <br><br><br>
</body>
</html>
)=====";

const char update[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
  <title>ESPbutton</title>
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=2.0, minimum-scale=0.10">
  <meta name='mobile-web-app-capable' content='yes'>
  <meta http-equiv="refresh" content="30; url=/">
  <meta name='theme-color' content='#000'>
  <link rel='icon' sizes='256x256' href='https://esp.3dstar.ro/favicon.png'>
</head>
<body style="background-color:#000;color:#fff;text-align:center;">
    <h2>ESPbutton</h2>
    <br>
    <h3>Performing firmware update...</h3>
    <br><br>
    <h3> Click button or wait for auto reload...</h3>
    <form action="/">
        <input type="submit" value="Main menu">
    </form>
    <br>
    <hr><br>
    <a href="https://3dstar.ro/">3DStar</a> -
    <a href="https://3dstar.ro/proiecte/espbutton">ESPbuton</a> -
    <a href="https://github.com/cctweaker/ESPbutton">GitHub</a>
    <br><br><br>
</body>
</html>
)=====";

const char page_header[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
  <title>ESPbutton</title>
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=2.0, minimum-scale=0.10">
  <meta name='mobile-web-app-capable' content='yes'>
  <meta name='theme-color' content='#000'>
  <link rel='icon' sizes='256x256' href='https://esp.3dstar.ro/favicon.png'>
</head>
<body style="background-color:#000;color:#fff;text-align:center;">
)=====";

const char page_header_mac[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
  <title>ESPbutton</title>
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=2.0, minimum-scale=0.10">
  <meta name='mobile-web-app-capable' content='yes'>
  <meta name='theme-color' content='#000'>
  <style>input[type=text]{width:2em;}</style>
  <link rel='icon' sizes='256x256' href='https://esp.3dstar.ro/favicon.png'>
</head>
<body style="background-color:#000;color:#fff;text-align:center;">
)=====";

const char page_footer[] PROGMEM = R"=====(
<hr><br>
<a href="https://3dstar.ro/">3DStar</a> -
<a href="https://3dstar.ro/proiecte/espbutton">ESPbutton</a> -
<a href="https://github.com/cctweaker/ESPbutton">GitHub</a>
<br><br><br>
</body>
</html>
)=====";

const char form_buttons[] PROGMEM = R"=====(
        <input type="submit" value="Submit">
        <br><br>
        <input type="submit" name="save" value="Save config">
    </form>
    <br>
    <form action="/">
        <input type="submit" value="Back">
    </form>
    <br>
)=====";

const char html_hr[] PROGMEM = R"=====(
<hr>
)=====";

const char html_table_i[] PROGMEM = R"=====(
<table align="center"><tr>
)=====";

const char html_table_s[] PROGMEM = R"=====(
</tr></table>
)=====";

const char html_table_tr[] PROGMEM = R"=====(
</tr><tr>
)=====";

const char html_table_tr_hr[] PROGMEM = R"=====(
</tr><tr><td colspan="2"><hr></td></tr><tr>
)=====";

const char form_start[] PROGMEM = R"=====(
<form>
)=====";

const char form_text_field_string[] PROGMEM = R"=====(
%s<br><input type="text" name="%s" value="%s"><br><br>
)=====";

const char form_text_field_string_min[] PROGMEM = R"=====(
%s<br><input type="text" name="%s" value="%s" minlength="1"><br><br>
)=====";

const char form_text_field_int[] PROGMEM = R"=====(
%s<br><input type="text" name="%s" value="%d"><br><br>
)=====";

const char form_text_field_int_min[] PROGMEM = R"=====(
%s<br><input type="text" name="%s" value="%d" minlength="1"><br><br>
)=====";

const char form_select_field[] PROGMEM = R"=====(
%s<br><select name="%s">
)=====";

const char form_select_field_end[] PROGMEM = R"=====(
</select><br><br>
)=====";

const char form_option_field[] PROGMEM = R"=====(
<option value="%d">%s</option>
)=====";

const char form_option_field_selected[] PROGMEM = R"=====(
<option value="%d" selected>%s</option>
)=====";

const char form_yes_no[] PROGMEM = R"=====(
%s<br><select name="%s"><option value="1" %s>yes</option><option value="0" %s>no</option></select><br><br>
)=====";

const char form_gmac_field[] PROGMEM = R"=====(
<input type="text" name="gmac%d" value="%02X" minlength="1">%s
)=====";

const char form_umac_field[] PROGMEM = R"=====(
<input type="text" name="umac%d" value="%02X" minlength="1">%s
)=====";

const char form_kok_field[] PROGMEM = R"=====(
<input type="text" name="kok%d" value="%02X" minlength="1">
)=====";

const char form_key_field[] PROGMEM = R"=====(
<input type="text" name="key%d" value="%02X" minlength="1">
)=====";

const char form_end[] PROGMEM = R"=====(
</form>
)=====";

const char menu_entry[] PROGMEM = R"=====(
<form action="%s"><input type="submit" value="%s" %s></form><br>
)=====";

const char menu_entry_check_restart[] PROGMEM = R"=====(
onSubmit="if(!confirm(\'Are you sure you wish to restart the device?\')){return false;}"
)=====";

const char menu_entry_check_erase[] PROGMEM = R"=====(
onSubmit="if(!confirm(\'Are you sure you wish to erase all settings?\')){return false;}"
)=====";

const char menu_entry_check_update[] PROGMEM = R"=====(
onSubmit="if(!confirm(\'Are you sure you wish to update the firmware?\')){return false;}"
)=====";