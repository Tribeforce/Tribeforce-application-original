// Copyright (c) 2013 Femi Veys


// Using the closure to map jQuery to $.
(function ($) {
  Drupal.behaviors.roles = {
    attach: function (context, settings) {
      var o = 'once-roles';

      /*
      Attach event handlers
      */

      // Simulate a click on the edit button like a click on the edit link
      $('.roles .edit').once(o).click(function() {
        $(this).parent().siblings('.edit-link').find('a').click();
      });

      // Simulate a click on the delete button like a click on the delete link
      $('.roles .delete').once(o).click(function() {
        $(this).parent().siblings('.delete-link').find('a').click();
      });

      // Simulate a click on the save button like a click on the save link
      $('.roles .save').once(o).click(function() {
        $(this).parent().siblings('.save-link').find('a').click();
      });

      // The level flames
      // Simulates a change in the hidden
      // TODO: Should selector be more specific or less?
      // TODO: live should maybe be replaced (needed for add )
      $('form .level div').live('click', function(event) {
        event.stopImmediatePropagation();
        var level = parseInt($(this).attr('class').substring(5,6));
        for(i=1; i<=level; i++) {
          $(this).parent().find('.level' + i).addClass('on');
        }

        for(i=level+1; i<=4; i++) {
          $(this).parent().find('.level' + i).removeClass('on');
        }

        // Set the level in the hidden dropdown
        var form = $(this).parents('form'); // Helper variable
        form.find('.field-name-field-expert-level select').val(level);
      });


      // The level flames for the talents
      $('.talent .level div').live('click', function(event) {
        event.stopImmediatePropagation();
        var level = parseInt($(this).attr('class').substring(5,6));
        $link = $(this).parents('.talent').find('.flame-links .level' + level + ' a');
        $link.click();
        $link.mousedown();
      });

      // Autocomplete Skill add
      // Copies the fields of the autocomplete items into the empty form
      // for that item
      // TODO: Watch out with the live() method:
      //       see http://api.jquery.com/live/
      // TODO: Generalize for other types
      $('.skills #autocomplete li').live('mousedown', function(event) {  // For skills only
        event.stopImmediatePropagation();
        $form = $(this).parents('form');
       // If a new empty skill is added, we need to populate only the title,
       // Otherwise we need to populate other fields in the form
       if($(this).find('.new-empty').length === 1) {
         $title = $(this).find('.new-empty .title').html();
       } else {
          $title = unhighlight($(this).find('.field-name-title .field-item')
                                                                     .html());
          // Body field
          $form.find('.field-name-body textarea').val(
            unhighlight($(this).find('.field-name-body .field-item').html())
          );
          // Level field
          $form.find('.field-name-field-expert-level select').val(
            get_level($(this).find('.level'))
          );
          $form.find('.level').replaceWith($(this).find('.level'));
       }

        // Title field
        $form.find('.form-item-title input').val($title);

        // Finally we need to switch the mode so the autocomplete field
        // disappears and the form fields appears
        $form.addClass('selected');
      });


      // Autocomplete Talent add
      // Copies the fields of the autocomplete items into the empty form
      // for that item
      // TODO: Watch out with the live() method:
      //       see http://api.jquery.com/live/
      // TODO: Generalize for other types
      $('.behavioral #autocomplete li, .business #autocomplete li').live('mousedown',
        function(event) {
          event.stopImmediatePropagation();
          $form = $(this).parents('form');

          //Set the color to white, the callback sets it back to black
          $form.find('.autocomplete input.form-autocomplete').css('color', '#fff');

          // Title field
          $form.find('.form-item-title input').val(
            unhighlight($(this).find('.field-name-title .field-item').html())
          );

          // Finally simulate a click on the submit button
          // We need to simulate this AJAX call by a mousedown event
          $form.find('.new input#edit-submit').mousedown();
        }
      );


      // When hitting ENTER some actions need to happen
      $(document).keypress(function(event) {
        if(event.keyCode == 13) {
          event.stopImmediatePropagation();
          // Simulate a mouseclick when hitting ENTER in the autocomplete list
          $selected = $('#autocomplete li.selected');
          if($selected.length > 0) {
            $selected.mousedown();
          }
        }
      });

      // When hitting ENTER or ESC in an input field some actions need to happen
      $('input').keydown(function(event) {
        switch(event.keyCode) {
          case 13: // The ENTER (keyCode = 13) actions
            event.stopImmediatePropagation();
            if(!$(event.target).hasClass('form-autocomplete')) {
              // Simulate a mouseclick when hitting ENTER in any input field
              $selected = $(this).parents('form').find('.actions .submit');
              if($selected.length > 0) {
                $selected.mousedown();
                $selected.click();
              }
            }
            break;
          case 27: // The ESCAPE (keyCode = 27) actions
            event.preventDefault();
            event.stopImmediatePropagation();
            // Simulate a mouseclick when hitting ESC in any input field
            $selected = $(this).parents('form').find('.actions .cancel').first();
            if($selected.length > 0) {
              $selected.mousedown();
              $selected.click();
            }
            break;
          }
      });


      $('.persons-widget .close').live('click', function(event) {
        event.stopImmediatePropagation();
        $(this).parents('.persons-widget').remove();
      });




      /*
      Helpers
      */
      function get_level($level) {
        var max_level = 0;
        $level.children().each(function() {
          if($(this).hasClass('on')) {
            max_level = Math.max(max_level, $(this).attr('class').substring(5, 6));
          }
        });
        return max_level;
      }

      // This hunction removes the highlight tags and also converts <br> and <p> to \n
      // TODO: Maybe this can be done in a nicer way
      function unhighlight(html) {
        var regex1 = new RegExp('<span class="highlight">', "g");
        var regex2 = new RegExp('</span>', "g");
        return $.trim(
                html.toString().replace(regex1, '')
                               .replace(regex2, '')
                               .replace(/(<p>)|(<\/p>)/g, "")
                               .replace(/(<br>)|(<br \/>)/g, "\r\n")
        );
      }

    }
  };



}(jQuery));
