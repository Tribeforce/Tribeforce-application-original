(function($){
  if (Drupal.settings.toolbar_hide.onload) {
    $(document).ready(
      function() {
        $('#toolbar').hide();
        $('body').addClass('tbr_menu_hidden');
    });
  }
  $(document.ready(
    function() {
      if ($.cookie('toolbar_hide_hidden')) {
        $('#toolbar').hide();
        $('body').addClass('tbr_menu_hidden');
      } else {
        $('#toolbar').show();
        $('body').removeClass('tbr_menu_hidden');
      }
    }
  ));
  $(document).keypress(function(e) {
    var unicode=e.keyCode? e.keyCode : e.charCode;
    if (String.fromCharCode(unicode)==Drupal.settings.toolbar_hide.key){
      $('#toolbar').slideToggle('fast');
      $('body').toggleClass('tbr_menu_hidden');
      if ($('body').hasClass('tbr_hide_hidden')) {
        $.cookie('toolbar_hide_hidden', 1);
      } else {
        $.cookie('toolbar_hide_hidden', 0);
      }
    }
  });
})(jQuery);
