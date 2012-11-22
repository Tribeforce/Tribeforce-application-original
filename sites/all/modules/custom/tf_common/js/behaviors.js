// Using the closure to map jQuery to $.
(function ($) {
  Drupal.behaviors.sidebar = {
    attach: function (context, settings) {
      $('.left-sidebar a.active').each(function() {
        $(this).siblings('.actions').addClass('active');
      });

      // Show/Hide functionality
      // TODO: Make selector generic
      $('.show-hide-header').click(function(event) {
        event.stopImmediatePropagation();
        $(this).parent().toggleClass('collapsed');
      });

      // Hide the slide pane
      $('.slide-pane .sp-close').click(function(event){
        event.stopImmediatePropagation();
        $slide_pane = $(this).parents('.slide-pane');
        $slide_pane.removeClass('open');
        setTimeout(function() {
          $slide_pane.remove();
          window.location.reload();
        }, 2000); // 5000 is too long but it won't ever be longer
      });

      // Get rid of all the containers for a date field
      $('.field-type-datetime').once(function() {
        $field = $(this).find('.form-type-textfield').children();
        $label = $(this).find('legend').html();
        $(this).children().remove();
        $(this).append($field);
        $(this).find('label').html($label);
      });


    }
  };
}(jQuery));
