
var style_cookie_name = "style" ;
var style_cookie_duration = 30 ;
var style_domain = "thecodevoid.com" ;

function switch_style ( css_title )
{

  var i, link_tag ;
  for(i = 0, link_tag = document.getElementsByTagName("link") ; i < link_tag.length ; i++ )
  {
    if((link_tag[i].rel.indexOf( "stylesheet" ) != -1) &&link_tag[i].title)
    {
      link_tag[i].disabled = true;
      if(link_tag[i].title == css_title)
        link_tag[i].disabled = false;
    }
    localStorage.setItem('Theme', css_title)
  }
}
function set_style_from_cookie()
{
  var css_title = localStorage.getItem('Theme')
  if(css_title.length)
    switch_style( css_title );
}

