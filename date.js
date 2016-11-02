var TodaysDateStyle = 6; //pick a style from below

/*---------------------------
Style 1: March 17, 2005
Style 2: Mar 17, 2005

Style 3: 17 March, 2005
Style 4: 17 Mar, 2005

Style 5: Saturday March 17, 2005
Style 6: Sat March 17, 2005
Style 7: Sat Mar 17, 2005

Style 8: Saturday 17 March, 2005
Style 9: Sat 17 March, 2005
Style 10: Sat 17 Mar, 2005

Style 11: 3/17/05
Style 12: 3/17/2005
Style 13: 17/3/05
Style 14: 17/3/2005

Style 15: Saturday March 17
Style 16: Saturday 17 March
----------------------------*/


function WriteTodaysDate(Style) {

var months = new Array();
months[1] = "January";  months[7] = "July";
months[2] = "February"; months[8] = "August";
months[3] = "March";    months[9] = "September";
months[4] = "April";    months[10] = "October";
months[5] = "May";      months[11] = "November";
months[6] = "June";     months[12] = "December";

var months2 = new Array();
months2[1] = "Jan"; months2[7] = "Jul";
months2[2] = "Feb"; months2[8] = "Aug";
months2[3] = "Mar"; months2[9] = "Sep";
months2[4] = "Apr"; months2[10] = "Oct";
months2[5] = "May"; months2[11] = "Nov";
months2[6] = "Jun"; months2[12] = "Dec";

var days = new Array();
days[1] = "Sunday";    days[5] = "Thursday";
days[2] = "Monday";    days[6] = "Friday";
days[3] = "Tuesday";   days[7] = "Saturday";
days[4] = "Wednesday";

var days2 = new Array();
days2[1] = "Sun"; days2[5] = "Thu";
days2[2] = "Mon"; days2[6] = "Fri";
days2[3] = "Tue"; days2[7] = "Sat";
days2[4] = "Wed";

var todaysdate = new Date();
var date  = todaysdate.getDate();
var day  = todaysdate.getDay() + 1;
var month = todaysdate.getMonth() + 1;
var yy = todaysdate.getYear();
var year = (yy < 1000) ? yy + 1900 : yy;
var year2 = year - (2000*1); year2 = (year2 < 10) ? "0" + year2 : year2;

var dateline = new Array();
dateline[1] = months[month] + " " + date + ", " + year;
dateline[2] = months2[month] + " " + date + ", " + year;
dateline[3] = date + " " + months[month] + ", " + year;
dateline[4] = date + " " + months2[month] + ", " + year;
dateline[5] = days[day] + " " + months[month] + " " + date + ", " + year;
dateline[6] = days2[day] + " " + months[month] + " " + date + ", " + year;
dateline[7] = days2[day] + " " + months2[month] + " " + date + ", " + year;
dateline[8] = days[day] + " " + date + " " + months[month] + ", " + year;
dateline[9] = days2[day] + " " + date + " " + months[month] + ", " + year;
dateline[10] = days2[day] + " " + date + " " + months2[month] + ", " + year;
dateline[11] = month + "/" + date + "/" + year2;
dateline[12] = month + "/" + date + "/" + year;
dateline[13] = date + "/" + month + "/" + year2;
dateline[14] = date + "/" + month + "/" + year;
dateline[15] = days[day] + " " + months[month] + " " + date;
dateline[16] = days[day] + " " + date + " " + months[month];

document.write(dateline[Style]);
}