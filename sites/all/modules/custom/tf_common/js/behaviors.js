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
      $('.form-wrapper.field-type-datetime').once(function() {
        $field = $(this).find('.form-type-textfield');
        $label = $(this).find('legend').html();
        $(this).children().remove();
        $(this).append($field);
        $(this).find('label').html($label);
      });
    }
  };

  Drupal.behaviors.tf_common = {
    attach: function (context, settings) {
      // TABS
      // Set the active tab logic
      $('.tab-pane .tabs div').click(function(event) {
        event.preventDefault();
        var tab = $(this).parents('.tab-pane').find('.panes .' +$(this).attr('class'));
        tab.addClass('active');
        tab.siblings().removeClass('active');

        $(this).addClass('active');
        $(this).siblings().removeClass('active');
        return false;
      });

      // Set the first pane and first tab in a tab-pane to active
      $('.tab-pane .tabs > div:first-child').addClass('active');
      $('.tab-pane .panes > div:first-child').addClass('active');

      // DROPDOWN
      // Clicking on the item or the dropdown icon slides the dropdown down
      $('.dropdown-widget .btn, .dropdown-widget .item').click(function(event) {
        event.stopImmediatePropagation();
        $(this).parents('.dropdown-widget').children().toggleClass('open');
      });

      // OVERLAY
      // Close the overlay
      $('.overlay .close').click(function(event) {
        event.stopImmediatePropagation();
        $(this).parents('.overlay').remove();
      });


    }
  };
}(jQuery));
