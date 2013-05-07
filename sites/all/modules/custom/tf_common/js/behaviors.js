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
        if($(this).parent().hasClass('collapsed')) {
          $(this).parent().find('.body').slideUp();
        } else {
          $(this).parent().find('.body').slideDown();
        }
      });

      // Hide collapsed bodies by default
      $('.collapsed').find('.body').hide();

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


      // IMPLEMENT THE GRACE PERIOD
      $(".tf-region.messages .grace-5").once(o, function() {
        var i;
        $secs = $(this).find('.secs');
        var ts = $(this).data('ts'); // Timestamp to be sure

        // Do the countdounwn
        for(i = 0; i < 5; i++) {
          grace_counter(i, $secs);
        }

        // Remove the message
        setTimeout(function() {
          // We need to make sure we are still selecting the same node.
          // It could have been removed / recreated in the meanwhile
          $selector = $('.tf-region.messages .grace-5.ts-' + ts);
          $selector.find('.start').click();
        }, 5000);
      });

      function grace_counter(i, $secs) {
        setTimeout(function() {$secs.html(5-i);}, i*1000);
      }

      // Bind the click event on the grace cancel link
      $(".tf-region.messages .grace-5 .cancel").click(function() {
        // Remove the action link so it cannot be clicked anymore
        $(this).parents('.grace-5').find('.start').remove();

        // Fade the message out and then remove the message all together
        $(this).parents('.grace-5').soft_remove();
      });

      // Whenever the start link is clicked (manually or by a script)
      // We need toi remove the message in a nice way
      $(".tf-region.messages .grace-5 .start").click(function() {
        $(this).parents('.grace-5').soft_remove();
      });


      // Handle global key events
      $(document).keyup(function(event) {
        $target = $(event.target);

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

      function fix_message_width() {
        var w = $('.tf-region.main').width();
        $('.tf-region.messages.stuck').width(w);
      }

      // SCROLL EVENTS
      $(window).scroll(fix_message_width);

      // RESIZE EVENTS
      $(window).resize(fix_message_width);



      // Make the messages sticky
      $('.tf-region.messages').waypoint('sticky');
      $('.sticky-wrapper').height('auto'); // We don't need this fix height
      setTimeout(fix_message_width, 500); // Nasty workaround


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


      /*******
      Invokers
      *******/
      $.fn.set_message = Drupal.behaviors.tf_common.set_message;
      $.fn.soft_remove = Drupal.behaviors.tf_common.soft_remove;


    },
    /*******
    Helpers
    *******/
    get_id_in_classes: function(classes) {
      var arr = classes.split(' ');
      var i;
      for(i = 0; i < arr.length; i++) {
        index = arr[i].indexOf("id-");
        if(index >= 0) {
          return arr[i].substring(index + 3, arr[i].length);
        }
      }
      return false;
    },

    set_message: function(msg, type) {
      var timestamp = Date.now();
      var classes =  'messages slide-down ' + type + ' ts-'+ timestamp;
      var selector = '.messages.'+ type + '.ts-'+ timestamp;
      var html = '<div class="' + classes + '">' + msg + '</div>';
      $('.tf-region.messages').append(html);
      $(selector).slideDown();
      setTimeout(function() {
        $(selector).soft_remove();
      }, 5000);
    },

    soft_remove: function() {
      $(this).slideUp(function() {
        $(this).remove();
      });
    }

  };
}(jQuery));
