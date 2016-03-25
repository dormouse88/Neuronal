[33mcommit 10f91ae19202f5b953c8537995c1f306453a8468[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Wed Feb 24 13:45:26 2016 +0000

    About to majorly refactor AGAIN. wtf is wrong with me.
    Going to introduce "Solder"s to encapsulate Tags and Weights rather than having pseudo-polymorphic wires.
    I may also introduce PortUnions as a separate object between Handles and Chips before the next commit.

[33mcommit 19dd8eb20f93cba4c060a99f6fec5e27c892c1cb[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Wed Feb 24 01:13:10 2016 +0000

    Refactored/fixed PlanName adding and removing.
    Fixed autoname generator for Tags as it wasn't exhaustive.
    Fixed port refreshing when loading plans.

[33mcommit a7a7020ca139693dd1fbc76fa6d9b172736f5f07[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Wed Feb 10 15:20:30 2016 +0000

    Added the TextEnterer class and eliminated the necessity of CursorsGoHere().
    TextEnterer accepts a std::bind function to determine the target action at object creation (before entry of text). It works too!

[33mcommit 286f058d51049f6783c9f314a83cd91ee0df5dc8[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Wed Feb 10 00:30:00 2016 +0000

    Moved recursion when saving plans from Serializer into Model.
    This is to allow PlanGroupData to be altered properly. I probably should make the same change for plan loading but I've not done so.

[33mcommit ebfd5d15b898108d5c57e072bd8d66eb993c54c7[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Tue Feb 9 20:08:27 2016 +0000

    More! Refactoring and bugfixing.
    UserData renamed to PlanGroupData and separated more from Serializer. Serializer now doesn't handle fine changes to PlanGroupData but just does a wholesale save instead.
    Also tried to typedef many various ints, strings, etc to better express types.

[33mcommit af7384e0c513b404ab4e41a245245a0aea68c2ad[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Sun Feb 7 16:18:36 2016 +0000

    A bit of minor refactoring and bugfixing.
    The innersim changes have now been broadly implemented and the biggest bugs removed. Still very rough though.
    Tried to fix TagCycle() method but ended up disabling that functionality for now instead.

[33mcommit d50bfc9dc8bdcc67d0f507d182ac2bd291055b09[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Sat Feb 6 17:56:04 2016 +0000

    Sort of implemented a named slots (now called Tags) system though it's still very buggy.
    Next thing to do has to be rethink structure and methods of object ownership.

[33mcommit 1ff7af691ca3a3912ad6ac28d2ee9cb673f7d772[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Fri Feb 5 16:06:19 2016 +0000

    Implemented a mostly-functional solution to "multiwires" issue.
    Outstanding issues/bugs include:
    adding wires to collapsed handles (could just disallow),
    modifying the tags on wires (and the cursor selection of that wire),
    adding a wire involving a port with the cursors reversed is highly surprising.

[33mcommit bc3c0476333aa0f08bae838710daa76b8bdc795b[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Wed Feb 3 14:57:52 2016 +0000

    Nothing much. Refactoring and a couple of bug-fixes.

[33mcommit ccbce4457361cc3437228f877409d1ccce20bb80[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Wed Jan 27 14:35:03 2016 +0000

    Made some progress on ports. They are visible and automatically sequenced according to the union of all connected tags. This applies so far to drawing, and wire attach pos (not selection).

[33mcommit 4b41cf0c570fe2b4e7ad24918aa1f8400bb5c374[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Sat Jan 23 21:34:14 2016 +0000

    Nothing much. Becoming a bit of a trend.

[33mcommit 1ce8e4350f32c0cb5f7731ba913d437ea222ca5b[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Sat Jan 23 17:51:48 2016 +0000

    Fixed a few broken things and a few charge refreshing issues.
    Started changing slot from type int to a typedef Tag for easier changing later.

[33mcommit 2fd3877c2fb00c592fe08e27082a143a93802f44[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Fri Jan 22 04:30:42 2016 +0000

    Spent ages "refactoring" but to little obvious effect other than to break a few things.

[33mcommit 5b1372c26ab4b6595c0efa9de27476a369dc57c9[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Thu Jan 21 14:53:58 2016 +0000

    Started work on changing the UI to accommodate plan ports.
    I'm about to chop PlanPos and Cursor up a bit which will have wide-reaching knock-on effects. I'm going to enforce that a PlanPos must have a valid position and not just be used to pass Plans around.

[33mcommit 81b03466296122ccfe74903a1c2f1dc999e8c9f1[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Wed Jan 20 00:00:45 2016 +0000

    Added some default device values in the serializer.
    Fixed a few bugs with the refresh system when loading plans.

[33mcommit 8f74daf777b5cc4a3cdf4f08935412ccc7b01370[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Tue Jan 19 22:22:25 2016 +0000

    Brilliant!
    The refresh system seems to handle fast-only loops well so they don't need special attention now.
    I've not done anything, just updated some docs.

[33mcommit 75401563f14021ae6a9ed58790340fe217b34528[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Tue Jan 19 22:09:48 2016 +0000

    Fully implemented charge refresh system and it works nicely.
    BaseReferer done.
    Refresh commands added for eg. deleting a wire.
    (Just need to choose a strategy for dealing with fast-only loops now.)

[33mcommit 0284a413da0e1adbd5d640f12cb8b798a307b32b[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Tue Jan 19 02:52:47 2016 +0000

    Largely implemented charge refresh system and it works nicely.
    BaseReferer still needs doing though, for example

[33mcommit a1fd62771b7770e9d60f94e039b9921f0a59d56e[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Mon Jan 18 22:56:23 2016 +0000

    Not much. Eliminated Jumpers fully, eliminated DeviceView,

[33mcommit 8705e659d5ce3755dcea907d9b3cfe6cce1a901b[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Mon Jan 18 17:55:47 2016 +0000

    Fairly successful in introducing the new 'fast-capable' neuron.
    Now draws bulbed (slow) neurons as two halves with independent firing states.
    Simple neurons are drawn smaller and without threshold text.
    Jumpers are now redundant but not fully purged.
    About to try out another idea for charge propagation based on refreshing changes.

[33mcommit 0dec032c57d6cd388e246595f4568da6bd48c052[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Sun Jan 17 15:42:03 2016 +0000

    Not much. About to hack things up a bit.
    XPuts are now highlighted when active.
    Added some documentation for the new neuron changes about to be made.
    Made a list of terminology to try to standardise it.

[33mcommit 56a71394aefa01951c23f41c9c7073aec512618e[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Wed Jan 13 03:32:36 2016 +0000

    Some things are now drawn better (Xputs of brains, Arena, Arena Spawners + Arena Entities)
    Improved 'physics' and fixed the glitch of cat and mouse being able to pass thru each other.
    Split some classes out of the Puppet files into Spawners and SpawnGroups files.

[33mcommit 438ee500594a5f361dae94f06fb72c80db5626b5[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Sun Jan 10 23:40:22 2016 +0000

    Levels are now loaded by reading level data from xml.

[33mcommit c48136c612185ce4776d85fd2937fa4288ccd469[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Sun Jan 10 03:37:00 2016 +0000

    I've got the cat moving around according to its brain now, so that's good.
    Made some decent progress towards reading level data from xml.
    Unfortunately, I changed BlobFactory from a namespace to a class and made a bit of a mess everywhere passing its instance around :(

[33mcommit 09bc779dd6f72eb764ac15bc4635052baf06fb4c[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Sat Jan 9 15:22:11 2016 +0000

    Some progress on the outergame. SpawnGroups, etc.

[33mcommit 7371d2ab89c5a7d780aaab341ac4b682413b6f27[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Wed Jan 6 00:41:35 2016 +0000

    Mainly some work on the design docs.
    I can't remember the source code changes. It's been a while.

[33mcommit aaf5805f12f143a2c5e90182fea309b0151f0996[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Thu Dec 17 14:20:51 2015 +0000

    Added an Arena and a rough class hierarchy for its inhabitants (ArenaStatic, ArenaMotile, etc).
    Puppet's act and sense functionality works, but it's all a bit rough still.

[33mcommit 4925ad83faaa0a5ae8cd84c1bb0a99ffd3f70b5d[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Sat Dec 12 01:07:42 2015 +0000

    More properly implemented some of the Puppet and BaseReferer functionality.
    I've removed the erroneous const declarations and can't be bothered adding proper const correctness back in.
    There's kind of a pink block that is now drawn to provide some representation of the outergame.

[33mcommit f5d5c47d9566f6fce91c952db07db9629e937d28[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Fri Dec 11 19:58:28 2015 +0000

    Introduced Puppet class.
    Also expanded BaseReferer class and moved it into its own hpp and cpp files.
    Lots of corners cut but it compiles and seems to start to run OK.
    Noticed some const mistakes though that I haven't addressed yet.

[33mcommit 474b2bafa73f1a48b5a37f007c1f400205480b37[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Thu Dec 10 15:09:08 2015 +0000

    Small change.
    Refactored SetPlan() and SwapIn(). They now better adhere to virtuals in RefererInterface.
    Organised and commented virtual functions better across several classes.

[33mcommit 948da76c9e0957d688c07d78dca4287ac2ccbd22[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Thu Dec 10 13:23:05 2015 +0000

    Added some more elements that get drawn on the bottom bar. They are all very crude at the moment but there's a textual input area, a plan ancestry panel and a message log that get drawn now.
    Also improved the planning documentation and moved it into the git repository.
    (The C++ of this commit is almost a week out of date. The source code hasn't much been changed in 5 days or so.)
    (Also, I'm about to start introducing the outergame section btw.)

[33mcommit 6a5135c8326dbe352d7b28c98c4cbe4f59216d9a[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Thu Nov 26 14:03:38 2015 +0000

    Textual entry now works for saving by name and load filtering (It's invisible since the text doesn't get drawn yet but it does work).

[33mcommit 62b140291c14330e07e18106b0fdf05bea668afe[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Wed Nov 25 03:30:55 2015 +0000

    Load Filtering now works and can cycle through planIDs, names and traverse ancestry using various numpad keys. Only thing missing is textual entry for naming/filtering.

[33mcommit a1c603001c9b1b093782ca1d28fbc6e067eef2ef[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Tue Nov 24 22:55:14 2015 +0000

    Moved some functions out of Serializer. Everything working again though. class UserData now acts as a wrapper to access Serializer. Names and ancestry is get and set through UserData.

[33mcommit da7593ee6b1e5ac99c8899b40ba12146fb2afc1b[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Tue Nov 24 16:59:44 2015 +0000

    Done some work on ancestry traversal.
    I'm about to move some functions out of Serializer. May break things a bit.

[33mcommit 642c0a1dea72adc19087be8245c7024a61fe40ad[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Mon Nov 23 00:39:08 2015 +0000

    Added serialization of plan names. Two save options are now available: SaveAsNew and Save(Replace). Names are transferred (to the replacement) and auto-names are randomly generated accordingly. Serializer class got refactored during this.
    Fixed bug when loading into the basePlan (oops).
    Disallowed deletion of handles that point to non-empty plans.

[33mcommit c6d579273fbc6b87931d3c26ef8c06aec8bb3b35[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Sat Nov 21 16:33:27 2015 +0000

    Added grabbers to allow selecting plans using just the mouse.
    ChipPlans now draw grid lines.
    Fixed bug stopping Handles of exploded plans from being moved.
    State of program is decent right now. Not too buggy.

[33mcommit 72890eb401ad4e2b455b9fc932fd9af3389eca2d[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Sat Nov 21 01:28:35 2015 +0000

    Ironed some cursor bugs.

[33mcommit dd69fe98e51ba488964d66abcea1ef48ae4512da[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Fri Nov 20 17:36:15 2015 +0000

    More restructuring. Controller cleaned. Serializer and Factory put inside model. Most functionality repaired.
    Handle/Plan cursor selection merged into one.

[33mcommit e84dc7b569ec7373da5ae7c10f2409ff8a93377d[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Thu Nov 19 16:06:08 2015 +0000

    Partially restructured to remove activePlan. Cursor heavily changed. Some functionality still broken.

[33mcommit fd75a8f88322f73587a2743eca04b7b7db1b642e[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Sun Nov 15 15:45:42 2015 +0000

    Cleaned exploded view implementation. No longer uses sf::View (each PlanGrid stores an offset).

[33mcommit 282e48209f386968bfdfe78e7d73829441fdd4f2[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Fri Nov 13 18:31:24 2015 +0000

    More more work and refactoring to make sub-plans display inside plans. Explodes successfully (works more than 1 layer deep, wires are correctly aligned). Still needs a little more doing to it (setting of padding).
    Plans now draw their number inside area at the bottom.

[33mcommit 897666d8ead9a138d4e5758339149f51fa670d8d[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Fri Nov 13 01:18:29 2015 +0000

    Further work and refactoring to make sub-plans display inside plans. Explodes with partial success (only works 1 layer deep, wires aren't correctly aligned).

[33mcommit a1bae30c601344a78e4bcf76ed02f8e4fd898afe[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Wed Nov 11 22:00:55 2015 +0000

    Preliminary support added for exploded plan view. Rows and columns within plans can be oversized and the coordinate system has been updated accordingly. Devices centre themselves within each cell.

[33mcommit 0fc0e1503f2c640a6645bde1ac8c7576df421900[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Tue Nov 10 23:56:02 2015 +0000

    Plan boundary is now drawn and view is clamped to stay on it properly. cursors can't select cells outside the boundary. Attempting to do so selects the plan itself. Wires joining the plan internally use the boundary as the attach point.
    Bottom bar now shows the stack of plans (from basePlan to activePlan).
    Loading and wiping plans now functions differently. + and - load the next consecutive plan in place, so long as current plan isn't modified.

[33mcommit 234b340469a6559bd54729283e6f04cceb0460a0[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Mon Nov 9 17:45:13 2015 +0000

    Now correctly detects modification of plans. Save/load respects this and modified plans are marked with an asterisk in the view.
    A few visual bugs fixed.

[33mcommit 46ffca4079a605eb693099731b66a4b889fe8069[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Thu Nov 5 16:55:59 2015 +0000

    Saving/Loading of plans added (albiet using primitive load selection).
    Slot values on wires are modifiable (and restrained correctly).

[33mcommit 1d8bf3aac3c51fa3635519aa75f87b7cd08d2ed8[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Wed Nov 4 13:13:47 2015 +0000

    Charges now pass up and down into ChipPlans.
    Two wires may no longer occupy the same route, and self-looping wires are disallowed. Users must use jumpers to achieve this functionality.
    Wire slot values can be changed.
    Wire drawing improved (added pulses and improved the text labels).
    View highlighting modes added (dims clutter).

[33mcommit 2d2516ed3a9a67e12a2398695bc300a60875ee06[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Mon Nov 2 14:40:07 2015 +0000

    Getting back into it.
    Need to get charges to pass up and down into ChipPlans.

[33mcommit 9b4267e4ef0d2874eb3b64cfa8919e788cb6cc12[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Fri Aug 28 19:20:41 2015 +0100

    Compiles again now and original functionality is restored (except serialization).
    Chip Handles can be added.
    Chip Plans don't work yet (except for the base plan).

[33mcommit 2ad50e45f7aaf5af12b660b122706b785923f3b2[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Tue Jul 28 00:26:12 2015 +0100

    In the middle of refactoring to include Chips.
    Everything has broken.
    I'm probably just about to tear down the MVC structure too.

[33mcommit 3bc7b7fc12f0d5786bdba8af848c38d1d18cce59[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Wed Jun 17 18:10:55 2015 +0100

    Finished serialization using pugixml.
    Started to add the Chip/Component classes.
    Integrating them could get messy soon.

[33mcommit 6503fe7505ffb623872c1a852abc22afe81ebf06[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Fri Apr 24 14:13:42 2015 +0100

    Added some basic serialization using pugixml.
    (loads and saves devices only)

[33mcommit e0e199752aa21b435bf4a7a024baa9975e49c4d4[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Thu Apr 23 11:54:46 2015 +0100

    Added sockets, including instant charge propagation and limiting input to one wire.

[33mcommit ff215b9fec4f53a7a6fa89abd95044a3509fe9b3[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Thu Apr 16 15:31:03 2015 +0100

    Still half-done adding sockets,
    Refactored to use factories.
    Extended MVC further. Now uses controller classes for objects.

[33mcommit 2f635c540386a3d06fc3a5000652f493da76f19c[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Wed Apr 8 19:01:00 2015 +0100

    Half-done adding sockets,
    About to refactor addition and deletion using factories.
    (Compiles but have introduced run-time bug if delete a neuron)

[33mcommit 4450d7f708bed91ee72b8f070f6a3f55f91caff9[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Mon Apr 6 03:27:25 2015 +0100

    Deleting neurons and wires added.
    Self-looping wires drawn properly.
    (Seems stable but code has gone quite messy)

[33mcommit cbbf93b2566c014d9e5b1aa5a20c39ebe528f4a9[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Fri Jan 30 22:39:28 2015 +0000

    Changing wire weights added.
    Camera Pan added.
    Moving neurons now checks space is free first.
    Wire direction indicated with colour.

[33mcommit 7a619175ea71c19cf669edfca2f108012e820452[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Mon Jan 19 21:33:01 2015 +0000

    MVC refactoring done (though a bit shabby)
    Dual-cursors added
    Moving neurons, adding wires, changing thresholds, etc. added.

[33mcommit af3d2173577487663dee6dbe86e10cdc5e44ef3f[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Fri Jan 16 21:18:29 2015 +0000

    Same functionality but largely refactored to MVC
    (it compiles but there's still some work to do on linking the model and view factories)

[33mcommit dfb61bb255a543bcdd55f068c8e1406b5dbfb3c4[m
Author: dormouse88 <dormouse88@users.noreply.github.com>
Date:   Thu Jan 15 18:00:05 2015 +0000

    Initial Commit
    (very basic simulation, input and graphics. Neuron and wires only)
