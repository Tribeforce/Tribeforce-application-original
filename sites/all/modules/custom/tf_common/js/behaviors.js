// Using the closure to map jQuery to $.
(function ($) {
  Drupal.behaviors.sidebar = {
    attach: function (context, settings) {
      var o = 'once-sidebar';

      $('.left-sidebar a.active').each(function() {
        $(this).siblings('.actions').addClass('active');
      });

      // Show/Hide functionality
      // TODO: Make selector generic
      $('.show-hide-header').once(o).click(function(event) {
        $(this).parent().toggleClass('collapsed');
      });

      // Hide the slide pane
      $('.slide-pane .sp-close').once(o).click(function(event){
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
      var o = 'once-common';

      // TABS
      // Set the active tab logic
      $('.tab-pane .tabs > div').once(o).click(function() {
        var class_name = $(this).attr('class').split(' ')[0];
        $pane = $(this).parents('.tab-pane').find('.panes .' + class_name);
        $pane.addClass('active');
        $pane.siblings().removeClass('active');

        $(this).addClass('active');
        $(this).siblings().removeClass('active');
      });

      // DROPDOWN
      // Clicking on the item or the dropdown icon slides the dropdown down
      $('.dropdown-widget > .btn, .dropdown-widget > .item').once(o)
        .click(function(event) {
          $(this).parents('.dropdown-widget').children().toggleClass('open');
        });

      $('.dropdown-widget .dropdown > div').once(o).click(function(event) {
        $widget = $(this).parents('.dropdown-widget');
        $widget.children().toggleClass('open');
        var txt = $(this).text();
        $widget.find('.item').text(txt);
      });

      // OVERLAY
      // Close the overlay
      $('.overlay .close').once(o).click(function(event) {
        $(this).parents('.overlay').fadeOut(function(){
          $(this).remove();
        });
      });

      // Handle global key events
      $(document).keyup(function(event) {
        $target = $(event.target)

        // If the maxlength has been reached, show a warning for 5 seconds
        if($target.attr("tagName") === 'INPUT') {
          event.stopImmediatePropagation();
          console.log('called');
          var max_length = $target.attr('maxlength');
          if($target.val().length >= max_length) {
            var msg = Drupal.t('This field can hold only @num characters',
                                                          {'@num': max_length});
            Drupal.behaviors.tf_common.set_message(msg, 'warning');
          }
        }

        // ESC closes the overlay
        $('.overlay').each(function() {
          if(event.which === 27) {
            $(this).fadeOut(function(){
              $(this).remove();
            });
          }
        });
      });

      // SCROLL EVENTS
      $(window).scroll(function() {
        $('.tf-region.messages.stuck').width($('.tf-region.main').width());
      });



      // Make the messages sticky
      $('.tf-region.messages').waypoint('sticky');
      $('.sticky-wrapper').height('auto'); // We don't need this Auto

      // ACTIVE LINKS
      var pathname = window.location.pathname;
      $('.tf-region.header .menu .tabs > div').each(function() {
        if(pathname.indexOf($(this).find('a').attr('href')) === 0) {
          $(this).addClass('active');
          $(this).find('a').addClass('active');
        }
      });

      // HEIGHT FIX
      var max_height = Math.max(
        $('.tf-region.main').height(),
        $('.tf-region.left-sidebar').height()
      );
      $('.tf-module').height(max_height);
    },
    /*******
    Helpers
    *******/
    get_nid_in_classes: function(classes) {
      var arr = classes.split(' ');
      var i;
      for(i = 0; i < arr.length; i++) {
        index = arr[i].indexOf("nid-");
        if(index >= 0) {
          return arr[i].substring(index + 4, arr[i].length);
        }
      }
      return false;
    },

    set_message: function(msg, type) {
      var timestamp = Date.now();
      var classes =  'messages ' + type + ' ts-'+ timestamp;
      var selector = '.messages.'+ type + '.ts-'+ timestamp;
      var html = '<div class="' + classes + '">' + msg + '</div>';
      $('.tf-region.messages').append(html);
      setTimeout(function() {
        $(selector).fadeOut(function() {
          $(this).remove();
        });
      }, 5000);
    }

  };
}(jQuery));
