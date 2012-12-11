// Using the closure to map jQuery to $.
(function ($) {
/*  Drupal.behaviors.role_selection = {
    attach: function (context, settings) {
      var roles_container_id = '#roles';
      var role_preview_id = '#preview';
      var role_picked_id = '#picked';
      var picked = $('[name=picked]');

      $('#edit-search-field').
        keyup(
          function() {
            var string = $(this).val();
            search_role(string);
          }
        );

      function reset_div(div_id) {
        switch(div_id) {
          case role_preview_id:
            $(div_id).html('<h2 class="no-items">'
                                          + Drupal.t('Preview Pane')+'</h2>');
            break;
          default:
            $(div_id).html('');
        }
      }

      function search_role(value) {
        var min_length = 2;

        if(value.length === 0) {
          reset_div(roles_container_id);
          reset_div(role_preview_id);
        } else if(value.length < min_length) {
          $(roles_container_id).html('I need at least '
                                                + min_length + ' characters');
        } else {
          $.ajax({
             type: "GET",
             url: "/role/search/autocomplete/"+encodeURI(value),
             dataType: "html",
             success: function(msg){$(roles_container_id).html(msg);}
          });
        }
      }

      // Show roles preview when click on a role
      $('#roles div, #picked div.picked').
        live('click',
          function() {
            var nid = $(this).attr('id');
            // TODO: fix: Assumed there is only 1 class
            var class_string = $(this).attr('class');
            show_role_preview(nid, class_string);
          }
        );

      function show_role_preview(nid, type) {
        $.ajax({
           type: "GET",
           url: "/role/preview/"+encodeURI(nid)+"/"+type,
           dataType: "html",
           success: function(msg){$(role_preview_id).html(msg);}
        });
      }

      // Actions for the buttons
      $('#preview .button').
        live('click',
          function() {
            var nid = $(this).attr('id');
            // TODO: fix: Assumed there is only 1 class
            var class_string = $(this).attr('class');
            switch(class_string) {
              case('button delete'):
                del_selection(nid);
                break;
              case('button buy'):
                add_selection(nid);
                break;
              case('button add'):
                add_selection(nid);
                break;
            }
          }
        );

      //// Actions to perform for adding a selection
      function add_selection(nid) {
        var separator = ',';
        var picked_string = picked.val();
        var picked_array = [];

        // To make sure we don't create an array with an empty element,
        //we only need to create the array if picked_string is not empty
        if(picked_string.length > 0) {
          picked_array = picked_string.split(separator);
        }

        // If element has already been added, highlight it
        if($.inArray(nid, picked_array) !== -1 ) {
          $(role_picked_id).children('#'+nid)
            .effect('highlight',{color: '#FF4000'}, 1000);
        }
        // If it had not been added:
        //   add it, update the hidden form field and upadete the picked div
        else {
          // This is where the support for picking and joining multiple roles
          // is disabled. Switch comment of the 2 lines below
          // to change the behavior
//          picked_array.push(nid);  // for multiple
          picked_array[0] = nid; // For 1

          picked_string = picked_array.join(separator);
          picked.val(picked_string);
          theme_picked(picked_string);
          reset_div(role_preview_id);
        }
      }

      //// Actions to perform for deleting a selection
      function del_selection(nid) {
        var separator = ',';
        var picked_string = picked.val();
        var picked_array = [];

        // To make sure we don't create an array with an empty element,
        //we only need to create the array if picked_string is not empty
        if(picked_string.length > 0) {
          picked_array = picked_string.split(separator);
        }

        // If the element is present
        //   remove it from the array, update the hidden form field
        //   upadete the picked div and reset the preview pane
        var pos_in_array = $.inArray(nid, picked_array);
        if(pos_in_array !== -1 ) {
          picked_array.splice(pos_in_array, 1); // Remove element
          picked_string = picked_array.join(separator);
          picked.val(picked_string);
          theme_picked(picked_string);
          reset_div(role_preview_id);
        }
      }

      //// Updates the HTML of the picked div
      function theme_picked(picked_string) {
        $.ajax({
          type: "GET",
          url: "/recruit/jobopening/new/role_selection/picked/"
                                                  + encodeURI(picked_string),
          dataType: "html",
          success: function(msg){
           if($(role_picked_id).children().first().is('.no-items')) {
             $(role_picked_id).html(msg);
           } else {
             $(role_picked_id).html(msg);
           }
          }
        });
      }

      // Helper functions
    }
  };
*/

  Drupal.behaviors.campaign = {
    attach: function (context, settings) {
      // By default focus on the first text input element
      // TODO: Refine
      $('.campaign-details input[type=text]').focus();

      // Change select in dropdown
      $('.campaign-details .dropdown .role').click(function(event) {
        event.stopImmediatePropagation();
        var nid = Drupal.behaviors.tf_common.get_nid_in_classes($(this).attr('class'));
        $('.campaign-details .field-name-field-role-ref select').val(nid);
        return false;
      });
    }
  };








  Drupal.behaviors.interview = {
    attach: function (context, settings) {
      var o = 'once-interview';

      // BIND EVENT HANDLERS

      // Clicking on the score buttons
      $('.interview .scores .score').click(function() {
        var score = eval($(this).html());

        // Set the score in the select box
        $(this).parents('.answer').find('.field-name-field-score select')
          .val(score);

        // Set the score in the done widget
        $(this).parents('.question-wrapper').find('.header .done .score')
          .html(score);

        // Set the style on the clicked item and remove from the others
        $(this).siblings().removeClass('selected');
        $(this).addClass('selected');
      });


      // Setting and unsetting the collapsed flag
      $('.interview .more, .interview .less, .interview .field-name-title')
        .click(function(event) {
          event.stopImmediatePropagation();

          var wrapper = $(this).parents('.question-wrapper');

          // When clicking "more", the item should become active again
          wrapper.removeClass('done');
          wrapper.addClass('activated');

          // The collapsed toggle
          $(this).parents('.question-wrapper').toggleClass('collapsed');
          $(this).parents('.question-wrapper').find('.answer-body').slideToggle();
      });


      // Setting done flag
      $('.interview .form-submit').once(o).mousedown(function(event) {
        var wrapper = $(this).parents('.question-wrapper');
        wrapper.addClass('collapsed done');
        wrapper.removeClass('activated');
        wrapper.find('.answer-body').slideToggle();
      });


      // Clicking the Done button
      $('.interview .person .done').click(function() {
        $(this).parents('.overlay').fadeOut(function() {
          $(this).remove();
        });
      });



    }
  };



  Drupal.behaviors.prepare_interview = {
    attach: function (context, settings) {
      // The header toggle for skills and Talent Attributes
      $('.prepare-interview:not(.locked) .switch').click(
        function(event) {
          // Very important to prevent flickering side effects
          event.stopImmediatePropagation();
console.log('switch clicked' + event);
          var item = $(this).parent().parent();
          var body = $(this).parent().next(); // Helper variable

          // We cannot close if children are active
          if(!(item.hasClass('on') && body.find('.ta.on').length > 0)) {
            // Toggle switch, Set flag, Simulate submission and Toggle body
            item.toggleClass('on');

            // Disable the functionality for Talents
            if(!item.hasClass('talent')) {
              body.find('input[name="assess"]').val(item.hasClass('on') ? 1:0);
              body.find('input[type="submit"]').mousedown();
            }

          }

          return false;
        }
      );

      // Disable all input fields when the interview is locked
      $('.prepare-interview.locked input').attr('disabled', 'disabled');
      $('.prepare-interview.locked textarea').attr('disabled', 'disabled');


/*
      // The header toggle for Talents
      $('.talent .header .switch').click(
        function(event) {
          var body = $(this).parent().next(); // Helper variable

          // Very important to prevent flickering side effects
          event.stopImmediatePropagation();

          // Toggle switch, Set flag, Simulate submission and Toggle body
          $(this).toggleClass('on');
//          body.find('input[name="assess"]').val($(this).hasClass('on') ? 1:0);
//          body.find('input[type="submit"]').mousedown();
          body.slideToggle(500);
        }
      );
*/
    }
  };


  Drupal.behaviors.candidates = {
    attach: function (context, settings) {
      // Attach event handlers
/*
      // Click the Add CV button
      $('.page-recruit-candidates .cv .button').click(function(event) {
        $(this).parents('form')
          .find('.field-name-field-cv .form-file').click();
      });

      // Click the Add Motivation lettter button
      $('.page-recruit-candidates .motivation .button').click(function(event){
        $(this).parents('form')
          .find('.field-name-field-motivation .form-file').click();
      });
*/
      // Select Candidate
      $('.page-recruit-candidates .candidate .left').click(function(event) {
        var max_selected = 3;

        // Set/unset the selected classes
        $candidate = $(this).parents('.candidate');
        if($candidate.hasClass('selected')) {
          $candidate.removeClass('selected');
        }
        else {
          $candidate.addClass('selected');
        }

        // The selected candidates in a jQuery array
        $selected = $('.page-recruit-candidates .candidate.selected');
        var num_selected = $selected.length;

        // If max are already selected, do nothing
        if(num_selected > max_selected) {
          $candidate.removeClass('selected');
          alert(Drupal.t("Not more than 3 can be selected"));
          return;
        }

        // Set the selected-x class on the selection-pane
        $('.selection-pane').attr('class', 'selection-pane');
        $('.selection-pane').addClass('selected-' + num_selected);

        // Loop over the selected candidates to set the hidden selected field
        // and update the Compare candidates link
        var selected_array = [];
//        var comp_link = $('.page-recruit-candidates .button.compare a');
//        var link_array = comp_link.attr('href').split('/compare/');

        $selected.each(function(index) {
          classes = $(this).attr('class');
          selected_array.push(Drupal.behaviors.tf_common.get_nid_in_classes(classes));
        });

//        comp_link.attr('href', link_array[0] + '/compare/' + selected_array);

        $('.page-recruit-candidates form input[name=selected]')
          .val(selected_array); // TODO: probably not needed anymore
      });
    }
  };






  Drupal.behaviors.edit_role = {
    attach: function (context, settings) {
      $('.edit-role .accordion').accordion({
        autoHeight: false,
        header: 'h3.accordion',
        active: false
      });

      $('.edit-role .inline-edit').click(function() {
        var classes = $(this).attr('class').split(' ');
        var nid;
        var c;

        for(c = 0; c < classes.length; c++) {
//        for(c in classes) {
          if(classes[c].substr(0,4) === 'nid-') {
            nid = classes[c].substr(4);
            break;
          }
        }

        $.ajax({
          url:'/ipe_ajax/'+nid,
                       type: "POST",
                       dataType: 'json',
                       contentType: 'application/x-www-form-urlencoded'
        });

      });


      // Make the Next button behave like submut for the talents part
      // The other parts are edited through AJAX
      $('.edit-role .form-submit').click(function() {
        $(this).prev().find('form').submit();
      });

      // For mouse click
      $('.edit-role .autocomplete-item').live('click', function() {
        $(this).parents('form').find('.form-submit').mousedown();
      });
      // For ENTER or ESC
      $('.edit-role .skills input.form-autocomplete')
        .live('keyup', function(event) {
          switch(event.which) {
              case 13: // ENTER
                $(this).parents('form').find('.form-submit').mousedown();
                break;
              case 27: // ESC
                $(this).parents('form').find('.form-item-title input').val('');
                break;
          }
        });
    }
  };

  Drupal.behaviors.compare = {
    attach: function (context, settings) {
      /**********
        Helpers
      **********/
      function set_cand_unchecked() {
        // The $(this) holds the jQuery checkbox element.
        // The id attribute of the checkbox holds the class of the column of
        // the candidate checked. Example: candidate-1234
        $candidates = $('.compare .' + $(this).attr('id'));

        if($(this).is(':checked')) {
          $candidates.removeClass('unchecked');
        }
        else {
          $candidates.addClass('unchecked');
        }
      }


      /*****************
        Event Handlers
      *****************/
      // Click on a row in the scoreboard
      $('.compare .score-board .row a').click(function() {
        $(this).parents('.row').addClass('active');
        $(this).parents('.row').siblings().removeClass('active');
      });

      // Change the checkbox for a candidate
      $('.compare input[type=checkbox]').change(set_cand_unchecked);

      // TODO: Maybe this can be done in 1 call
      // If only 1 checkbox is left checked, we need to change the
      // checkbox in a Hire button
      $('.compare input[type=checkbox]').change(function(event) {
        event.stopImmediatePropagation();
        if($('.compare input[type=checkbox]:checked').length === 1) {
          var cand_class = $('.compare input[type=checkbox]:checked')
                                                                  .attr('id');
          $('.compare .' + cand_class).addClass('hire');
        }
        else {
          $('.compare .col').removeClass('hire');
        }
      });


      /**********
        Invokers
      **********/
      // This function is invoked by ajax_command_invoke
      // It finds the unchecked candidates and adds the "unchecked" classs
      $.fn.invoke_unselect_candidates = function() {
        $('.compare input[type=checkbox]').each(set_cand_unchecked);
      };
    }
  };
}(jQuery));
