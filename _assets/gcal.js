$().ready(function() {
    var b21 = '7hiop9tvins97kal7flob282e8@group.calendar.google.com';
    var id = 'cs.stanford.edu_nhpcljcvd03ub5s8rkf14o3vig@group.calendar.google.com'
    var base = 'https://www.googleapis.com/calendar/v3/calendars/' + id + '/events?key=AIzaSyAswReDophbZgMu3NnkPp6KV27Or1u2rS4';
    var min = "&timeMin=" + new Date().toISOString();
    var dayms = 1000*60*60*24; // convert to ms
    var max = "&timeMax=" + new Date(Date.now() + 8*dayms).toISOString(); 
    var options = '&singleEvents=true&orderBy=startTime&sortorder=ascending' + min + max;
    console.log("url is " + base + options)
    $.getJSON(base + options, function(data){
      var html = "";
      var prevDate = undefined;
      $.each(data.items, function(i, event){

        if (event.summary) {  // event w/ private visible has no summary, skip it

            var date_fmt = { weekday:"short", year:"numeric", month:"short", day:"numeric" };
            var time_fmt = { hour:"numeric", minute:"2-digit" };
            var start = new Date(event.start.dateTime);
            var end = new Date(event.end.dateTime);

            html += '<li class="list-group-item">'
                + (start.getDate() != prevDate ? start.toLocaleDateString("en-US", date_fmt) : "Cont'd")
                + " <b>" + event.summary + "</b> "
                + start.toLocaleTimeString("en-US", time_fmt) + '-' + end.toLocaleTimeString("en-US", time_fmt)
                + " in " + event.location
                + "</li>";
            prevDate = start.getDate();
        }
      });
      $('#upcoming ul').empty().append(html);  
    });
});