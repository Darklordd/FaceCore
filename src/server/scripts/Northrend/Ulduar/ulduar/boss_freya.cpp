/*
 * Copyright (C) 2008-2011 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "ulduar.h"

/*
 * **** TODO ****
 * Achievements *
 */

enum FreyaYells
{
    // Freya
    SAY_AGGRO                                    = -1603180,
    SAY_AGGRO_WITH_ELDER                         = -1603181,
    SAY_SLAY_1                                   = -1603182,
    SAY_SLAY_2                                   = -1603183,
    SAY_DEATH                                    = -1603184,
    SAY_BERSERK                                  = -1603185,
    SAY_SUMMON_CONSERVATOR                       = -1603186,
    SAY_SUMMON_TRIO                              = -1603187,
    SAY_SUMMON_LASHERS                           = -1603188,
    SAY_YS_HELP                                  = -1603189,

    // Elder Brightleaf
    SAY_BRIGHTLEAF_AGGRO                         = -1603190,
    SAY_BRIGHTLEAF_SLAY_1                        = -1603191,
    SAY_BRIGHTLEAF_SLAY_2                        = -1603192,
    SAY_BRIGHTLEAF_DEATH                         = -1603193,

    // Elder Ironbranch
    SAY_IRONBRANCH_AGGRO                         = -1603194,
    SAY_IRONBRANCH_SLAY_1                        = -1603195,
    SAY_IRONBRANCH_SLAY_2                        = -1603196,
    SAY_IRONBRANCH_DEATH                         = -1603197,

    // Elder Stonebark
    SAY_STONEBARK_AGGRO                          = -1603198,
    SAY_STONEBARK_SLAY_1                         = -1603199,
    SAY_STONEBARK_SLAY_2                         = -1603200,
    SAY_STONEBARK_DEATH                          = -1603201,
};

enum FreyaSpells
{
    // Freya
    SPELL_ATTUNED_TO_NATURE                      = 62519,
    SPELL_TOUCH_OF_EONAR                         = 62528,
    SPELL_SUNBEAM                                = 62623,
    SPELL_ENRAGE                                 = 47008,
    SPELL_FREYA_GROUND_TREMOR                    = 62437,
    SPELL_ROOTS_FREYA                            = 62283,
    SPELL_STONEBARK_ESSENCE                      = 62483,
    SPELL_IRONBRANCH_ESSENCE                     = 62484,
    SPELL_BRIGHTLEAF_ESSENCE                     = 62485,
    SPELL_DRAINED_OF_POWER                       = 62467,
    SPELL_SUMMON_EONAR_GIFT                      = 62572,

    // Stonebark
    SPELL_FISTS_OF_STONE                         = 62344,
    SPELL_GROUND_TREMOR                          = 62325,
    SPELL_PETRIFIED_BARK                         = 62337,
    SPELL_PETRIFIED_BARK_DMG                     = 62379,

    // Ironbranch
    SPELL_IMPALE                                 = 62310,
    SPELL_ROOTS_IRONBRANCH                       = 62438,
    SPELL_THORN_SWARM                            = 62285,

    // Brightleaf
    SPELL_FLUX_AURA                              = 62239,
    SPELL_FLUX                                   = 62262,
    SPELL_FLUX_PLUS                              = 62251,
    SPELL_FLUX_MINUS                             = 62252,
    SPELL_SOLAR_FLARE                            = 62240,
    SPELL_UNSTABLE_SUN_BEAM_SUMMON               = 62207, // Trigger 62221

    // Stack Removing of Attuned to Nature
    SPELL_REMOVE_25STACK                         = 62521,
    SPELL_REMOVE_10STACK                         = 62525,
    SPELL_REMOVE_2STACK                          = 62524,

    // Wave summoning spells
    SPELL_SUMMON_LASHERS                         = 62687,
    SPELL_SUMMON_TRIO                            = 62686,
    SPELL_SUMMON_ANCIENT_CONSERVATOR             = 62685,

    // Detonating Lasher
    SPELL_DETONATE                               = 62598,
    SPELL_FLAME_LASH                             = 62608,

    // Ancient Water Spirit
    SPELL_TIDAL_WAVE                             = 62653,
    SPELL_TIDAL_WAVE_EFFECT                      = 62654,

    // Storm Lasher
    SPELL_LIGHTNING_LASH                         = 62648,
    SPELL_STORMBOLT                              = 62649,

    // Snaplasher
    SPELL_HARDENED_BARK                          = 62664,
    SPELL_BARK_AURA                              = 62663,

    // Ancient Conservator
    SPELL_CONSERVATOR_GRIP                       = 62532,
    SPELL_NATURE_FURY                            = 62589,
    SPELL_SUMMON_PERIODIC                        = 62566,
    SPELL_SPORE_SUMMON_NW                        = 62582, // Not used, triggered by SPELL_SUMMON_PERIODIC
    SPELL_SPORE_SUMMON_NE                        = 62591,
    SPELL_SPORE_SUMMON_SE                        = 62592,
    SPELL_SPORE_SUMMON_SW                        = 62593,

    // Healthly Spore
    SPELL_HEALTHY_SPORE_VISUAL                   = 62538,
    SPELL_GROW                                   = 62559,
    SPELL_POTENT_PHEROMONES                      = 62541,

    // Eonar's Gift
    SPELL_LIFEBINDERS_GIFT                       = 62584,
    SPELL_PHEROMONES                             = 62619,
    SPELL_EONAR_VISUAL                           = 62579,

    // Nature Bomb
    SPELL_NATURE_BOMB                            = 64587,
    SPELL_OBJECT_BOMB                            = 64600,
    SPELL_SUMMON_NATURE_BOMB                     = 64606,

    // Unstable Sun Beam
    SPELL_UNSTABLE_SUN_BEAM                      = 62211,
    SPELL_UNSTABLE_ENERGY                        = 62217,
    SPELL_PHOTOSYNTHESIS                         = 62209,
    SPELL_UNSTABLE_SUN_BEAM_TRIGGERED            = 62243,
    SPELL_FREYA_UNSTABLE_SUNBEAM                 = 62450, // Or maybe 62866?

    // Sun Beam
    SPELL_FREYA_UNSTABLE_ENERGY                  = 62451,
    SPELL_FREYA_UNSTABLE_ENERGY_VISUAL           = 62216,

    // Attuned To Nature spells
    SPELL_ATTUNED_TO_NATURE_2_DOSE_REDUCTION     = 62524,
    SPELL_ATTUNED_TO_NATURE_10_DOSE_REDUCTION    = 62525,
    SPELL_ATTUNED_TO_NATURE_25_DOSE_REDUCTION    = 62521,
};

enum FreyaNpcs
{
    NPC_SUN_BEAM                                 = 33170,
    NPC_DETONATING_LASHER                        = 32918,
    NPC_ANCIENT_CONSERVATOR                      = 33203,
    NPC_ANCIENT_WATER_SPIRIT                     = 33202,
    NPC_STORM_LASHER                             = 32919,
    NPC_SNAPLASHER                               = 32916,
    NPC_NATURE_BOMB                              = 34129,
    NPC_EONARS_GIFT                              = 33228,
    NPC_HEALTHY_SPORE                            = 33215,
    NPC_UNSTABLE_SUN_BEAM                        = 33050,
    NPC_IRON_ROOTS                               = 33088,
    NPC_STRENGTHENED_IRON_ROOTS                  = 33168,

    OBJECT_NATURE_BOMB                           = 194902,
};

enum FreyaActions
{
    ACTION_ELDER_DEATH                           = 1,
};

enum FreyaEvents
{
    // Freya
    EVENT_WAVE                                   = 1,
    EVENT_EONAR_GIFT                             = 2,
    EVENT_NATURE_BOMB                            = 3,
    EVENT_UNSTABLE_ENERGY                        = 4,
    EVENT_STRENGTHENED_IRON_ROOTS                = 5,
    EVENT_GROUND_TREMOR                          = 6,
    EVENT_SUNBEAM                                = 7,
    EVENT_ENRAGE                                 = 8,

    // Elder Stonebark
    EVENT_TREMOR                                 = 9,
    EVENT_BARK                                   = 10,
    EVENT_FISTS                                  = 11,

    // Elder Ironbranch
    EVENT_IMPALE                                 = 12,
    EVENT_IRON_ROOTS                             = 13,
    EVENT_THORN_SWARM                            = 14,

    // Elder Brightleaf
    EVENT_SOLAR_FLARE                            = 15,
    EVENT_UNSTABLE_SUN_BEAM                      = 16,
    EVENT_FLUX                                   = 17,
};

#define WAVE_TIME                                60000 // Normal wave is one minute
#define TIME_DIFFERENCE                          10000 // If difference between waveTime and WAVE_TIME is bigger then TIME_DIFFERENCE, schedule EVENT_WAVE in 10 seconds

class npc_iron_roots : public CreatureScript
{
    public:
        npc_iron_roots() : CreatureScript("npc_iron_roots") { }

        struct npc_iron_rootsAI : public Scripted_NoMovementAI
        {
            npc_iron_rootsAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                me->ApplySpellImmune(0, IMMUNITY_ID, 49560, true); // Death Grip
                me->setFaction(14);
                me->SetReactState(REACT_PASSIVE);
                summonerGUID = 0;
            }

            uint64 summonerGUID;

            void IsSummonedBy(Unit* summoner)
            {
                // Summoner is a player, who should have root aura on self
                summonerGUID = summoner->GetGUID();
                me->SetFacingToObject(summoner);
                me->SetInCombatWith(summoner);
            }

            void JustDied(Unit* /*who*/)
            {
                if (Unit* target = ObjectAccessor::GetPlayer(*me, summonerGUID))
                {
                    target->RemoveAurasDueToSpell(SPELL_ROOTS_IRONBRANCH);
                    target->RemoveAurasDueToSpell(SPELL_ROOTS_FREYA);
                }

                me->RemoveCorpse(false);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_iron_rootsAI(creature);
        }
};

class boss_freya : public CreatureScript
{
    public:
        boss_freya() : CreatureScript("boss_freya") { }

        struct boss_freyaAI : public BossAI
        {
            boss_freyaAI(Creature* creature) : BossAI(creature, BOSS_FREYA)
            {
            }

            uint64 ElementalGUID[3][2];

            uint32 deforestation[6][2];
            uint32 elementalTimer[2];
            uint32 diffTimer;
            uint32 waveTime;
            uint8 trioWaveCount;
            uint8 trioWaveController;
            uint8 waveCount;
            uint8 elderCount;
            uint8 attunedToNature;

            bool checkElementalAlive[2];
            bool trioDefeated[2];
            bool waveInProgress;
            bool deforestationCheck;
            bool random[3];

            void Reset()
            {
                _Reset();
                summons.clear();
                waveTime = 0;
                trioWaveCount = 0;
                trioWaveController = 0;
                waveCount = 0;
                elderCount = 0;

                for (uint8 i = 0; i < 3; ++i)
                    for (uint8 n = 0; n < 2; ++n)
                        ElementalGUID[i][n] = 0;
                for (uint8 i = 0; i < 6; ++i)
                    for (uint8 n = 0; n < 2; ++n)
                        deforestation[i][n] = 0;
                for (uint8 n = 0; n < 2; ++n)
                {
                    checkElementalAlive[n] = true;
                    trioDefeated[n] = false;
                }
                waveInProgress = false;
                deforestationCheck = false;
                for (uint8 n = 0; n < 3; ++n)
                    random[n] = false;
            }

            void KilledUnit(Unit* /*who*/)
            {
                DoScriptText(RAND(SAY_SLAY_1, SAY_SLAY_2), me);
            }

            void DamageTaken(Unit* /*who*/, uint32& damage)
            {
                if (damage >= me->GetHealth() && instance)
                {
                    damage = 0;
                    DoScriptText(SAY_DEATH, me);
                    me->SetReactState(REACT_PASSIVE);
                    _JustDied();
                    me->RemoveAllAuras();
                    me->AttackStop();
                    me->setFaction(35);
                    me->DeleteThreatList();
                    me->CombatStop(true);
                    me->ForcedDespawn(7500);

                    Creature* Elder[3];
                    for (uint8 n = 0; n < 3; ++n)
                    {
                        Elder[n] = ObjectAccessor::GetCreature(*me, instance->GetData64(BOSS_BRIGHTLEAF + n));
                        if (Elder[n] && Elder[n]->isAlive())
                        {
                            Elder[n]->setFaction(35);
                            Elder[n]->RemoveAllAuras();
                            Elder[n]->AttackStop();
                            Elder[n]->CombatStop(true);
                            Elder[n]->DeleteThreatList();
                        }
                    }
                }
            }

            void EnterCombat(Unit* who)
            {
                _EnterCombat();
                DoZoneInCombat();
                Creature* Elder[3];
                for (uint8 n = 0; n < 3; ++n)
                {
                    Elder[n] = ObjectAccessor::GetCreature(*me, instance->GetData64(BOSS_BRIGHTLEAF + n));
                    if (Elder[n] && Elder[n]->isAlive())
                    {
                        me->AddAura(SPELL_DRAINED_OF_POWER, Elder[n]);
                        Elder[n]->CastSpell(me, SPELL_IRONBRANCH_ESSENCE, true);
                        Elder[n]->RemoveLootMode(LOOT_MODE_DEFAULT);
                        Elder[n]->AI()->AttackStart(who);
                        Elder[n]->AddThreat(who, 250.0f);
                        Elder[n]->SetInCombatWith(who);
                        ++elderCount;
                    }
                }

                if (Elder[0]->isAlive())
                {
                    Elder[0]->CastSpell(me, SPELL_BRIGHTLEAF_ESSENCE, true);
                    events.ScheduleEvent(EVENT_UNSTABLE_ENERGY, urand(10000, 20000));
                }
                if (Elder[1]->isAlive())
                {
                    Elder[1]->CastSpell(me, SPELL_STONEBARK_ESSENCE, true);
                    events.ScheduleEvent(EVENT_GROUND_TREMOR, urand(10000, 20000));
                }
                if (Elder[2]->isAlive())
                {
                    Elder[2]->CastSpell(me, SPELL_IRONBRANCH_ESSENCE, true);
                    events.ScheduleEvent(EVENT_STRENGTHENED_IRON_ROOTS, urand(10000, 20000));
                }

                if (elderCount == 0)
                    DoScriptText(SAY_AGGRO, me);
                else
                    DoScriptText(SAY_AGGRO_WITH_ELDER, me);

                me->CastCustomSpell(SPELL_ATTUNED_TO_NATURE, SPELLVALUE_AURA_STACK, 150, me, true);

                events.ScheduleEvent(EVENT_WAVE, 10000);
                events.ScheduleEvent(EVENT_EONAR_GIFT, 25000);
                events.ScheduleEvent(EVENT_ENRAGE, 600000);
                events.ScheduleEvent(EVENT_SUNBEAM, urand(5000, 15000));
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STAT_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_ENRAGE:
                            DoScriptText(SAY_BERSERK, me);
                            DoCast(me, SPELL_ENRAGE);
                            break;
                        case EVENT_SUNBEAM:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                DoCast(target, SPELL_SUNBEAM);
                            events.ScheduleEvent(EVENT_SUNBEAM, urand(10000, 15000));
                            break;
                        case EVENT_NATURE_BOMB:
                        {
                            // On every player
                            std::list<Player*> PlayerList;
                            Trinity::AnyPlayerInObjectRangeCheck checker(me, 50.0f);
                            Trinity::PlayerListSearcher<Trinity::AnyPlayerInObjectRangeCheck> searcher(me, PlayerList, checker);
                            me->VisitNearbyWorldObject(50.0f, searcher);
                            for (std::list<Player*>::const_iterator itr = PlayerList.begin(); itr != PlayerList.end(); ++itr)
                                (*itr)->CastSpell(*itr, SPELL_SUMMON_NATURE_BOMB, true);
                            events.ScheduleEvent(EVENT_NATURE_BOMB, urand(10000, 12000));
                            break;
                        }
                        case EVENT_UNSTABLE_ENERGY:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                DoCast(target, SPELL_FREYA_UNSTABLE_SUNBEAM, true);
                            events.ScheduleEvent(EVENT_UNSTABLE_ENERGY, urand(15000, 20000));
                            break;
                        case EVENT_WAVE:
                            waveTime = 0;
                            waveInProgress = true;
                            SpawnWave();
                            if (waveCount < 6)
                                events.ScheduleEvent(EVENT_WAVE, WAVE_TIME);
                            else
                                events.ScheduleEvent(EVENT_NATURE_BOMB, urand(10000, 20000));
                            break;
                        case EVENT_EONAR_GIFT:
                            DoCast(me, SPELL_SUMMON_EONAR_GIFT);
                            events.ScheduleEvent(EVENT_EONAR_GIFT, urand(40000, 50000));
                            break;
                        case EVENT_STRENGTHENED_IRON_ROOTS:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true, -SPELL_ROOTS_FREYA))
                                target->CastSpell(target, SPELL_ROOTS_FREYA, true); // This must be casted by Target self
                            events.ScheduleEvent(EVENT_STRENGTHENED_IRON_ROOTS, urand(12000, 20000));
                            break;
                        case EVENT_GROUND_TREMOR:
                            DoCastAOE(SPELL_FREYA_GROUND_TREMOR);
                            events.ScheduleEvent(EVENT_GROUND_TREMOR, urand(25000, 28000));
                            break;
                    }
                }

                if (!me->HasAura(SPELL_TOUCH_OF_EONAR))
                    me->CastSpell(me, SPELL_TOUCH_OF_EONAR, true);

                // For achievement check
                if (Aura* aura = me->GetAura(SPELL_ATTUNED_TO_NATURE))
                    attunedToNature = aura->GetStackAmount();
                else
                    attunedToNature = 0;

                diffTimer += diff;                                               // For getting time difference for Deforestation achievement

                if (waveInProgress)
                    waveTime += diff;                                            // Time from the last wave

                // Elementals must be killed within 12 seconds of each other, or they will all revive and heal
                Creature* Elemental[3][2];
                for (uint8 i = 0; i < 2; ++i)
                {
                    if (checkElementalAlive[i])
                        elementalTimer[i] = 0;
                    else
                    {
                        elementalTimer[i] += diff;
                        for (uint8 k = 0; k < 3; ++k)
                            Elemental[k][i] = ObjectAccessor::GetCreature(*me, ElementalGUID[k][i]);
                        if (elementalTimer[i] > 12000)
                        {
                            if (!trioDefeated[i]) // Do *NOT* merge this bool with bool few lines below!
                            {
                                if (Elemental[0][i] && Elemental[1][i] && Elemental[2][i])
                                {
                                    for (uint8 n = 0; n < 3; ++n)
                                    {
                                        if (Elemental[n][i]->isAlive())
                                            Elemental[n][i]->SetHealth(Elemental[n][i]->GetMaxHealth());
                                        else
                                            Elemental[n][i]->Respawn();
                                    }
                                }
                            }
                            checkElementalAlive[i] = true;
                        }
                        else
                        {
                            if (!trioDefeated[i])
                                if (Elemental[0][i] && Elemental[1][i] && Elemental[2][i])
                                    if (Elemental[0][i]->isDead() && Elemental[1][i]->isDead() && Elemental[2][i]->isDead())
                                    {
                                        for (uint8 n = 0; n < 3; ++n)
                                        {
                                            summons.remove(Elemental[n][i]->GetGUID());
                                            Elemental[n][i]->ForcedDespawn(5000);
                                            trioDefeated[i] = true;
                                            Elemental[n][i]->CastSpell(me, SPELL_REMOVE_10STACK, true);
                                        }
                                        TimeCheck();
                                    }
                        }
                    }
                }

                DoMeleeAttackIfReady();
            }

            // Check if all Trio NPCs are dead - achievement check
            void LasherDead(uint32 type)                                         // Type must be in format of a binary mask
            {
                uint8 n = 0;

                // Handling recieved data
                for (uint8 i = 0; i < 5; ++i)                                    // We have created "instances" for keeping informations about last 6 death lashers - needed because of respawning
                {
                    deforestation[i][0] = deforestation[(i + 1)][0];             // Time
                    deforestation[i][1] = deforestation[(i + 1)][1];             // Type
                }
                deforestation[5][0] = diffTimer;
                deforestation[5][1] = type;

                // Check for achievement completion
                if (deforestation[0][1])                                         // Check for proper functionality of binary masks (overflow would not be problem)
                {
                    for (uint8 i = 0; i < 6; ++i)                                // Count binary mask
                    {
                        n += deforestation[i][1];
                    }
                    if ((deforestation[5][0] - deforestation[0][0]) < 10000)     // Time check
                    {
                        if (n == 14 && instance)                                 // Binary mask check - verification of lasher types
                        {
                            deforestationCheck = true;
                        }
                    }
                }
            }

            // Random order of spawning waves
            int GetWaveId()
            {
                if (random[0] && random[1] && random[2])
                    for (uint8 n = 0; n < 3; ++n)
                        random[n] = false;

                uint8 randomId = urand(0, 2);

                while (random[randomId])
                    randomId = urand(0, 2);

                random[randomId] = true;
                return randomId;
            }

            void SpawnWave()
            {
                switch (GetWaveId())
                {
                    case 0:
                        DoScriptText(SAY_SUMMON_LASHERS, me);
                        for (uint8 n = 0; n < 10; ++n)
                            DoCast(SPELL_SUMMON_LASHERS);
                        break;
                    case 1:
                        DoScriptText(SAY_SUMMON_TRIO, me);
                        DoCast(SPELL_SUMMON_TRIO);
                        trioWaveCount++;
                        break;
                    case 2:
                        DoScriptText(SAY_SUMMON_CONSERVATOR, me);
                        DoCast(SPELL_SUMMON_ANCIENT_CONSERVATOR);
                        break;
                }
                waveCount++;
            }

            void JustSummoned(Creature* summoned)
            {
                switch (summoned->GetEntry())
                {
                    case NPC_SNAPLASHER:
                    case NPC_ANCIENT_WATER_SPIRIT:
                    case NPC_STORM_LASHER:
                        ElementalGUID[trioWaveController][trioWaveCount] = summoned->GetGUID();
                        summons.push_back(summoned->GetGUID());
                        trioWaveController++;
                        if (trioWaveController > 2)
                            trioWaveController = 0;
                        break;
                    case NPC_DETONATING_LASHER:
                    case NPC_ANCIENT_CONSERVATOR:
                        summons.push_back(summoned->GetGUID());
                        break;
                }

                // Need to have it there, or summoned units would do nothing untill attacked
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 250.0f, true))
                {
                    summoned->AI()->AttackStart(target);
                    summoned->AddThreat(target, 250.0f);
                    DoZoneInCombat(summoned);
                }
            }

            void SummonedCreatureDies(Creature* summoned, Unit* who)
            {
                switch (summoned->GetEntry())
                {
                    case NPC_DETONATING_LASHER:
                        summoned->CastSpell(me, SPELL_REMOVE_2STACK, true);
                        summoned->CastSpell(who, SPELL_DETONATE, true);
                        summoned->ForcedDespawn(5000);
                        summons.remove(summoned->GetGUID());
                        TimeCheck();
                        break;
                    case NPC_ANCIENT_CONSERVATOR:
                        summoned->CastSpell(me, SPELL_REMOVE_25STACK, true);
                        summoned->ForcedDespawn(5000);
                        summons.remove(summoned->GetGUID());
                        TimeCheck();
                        break;
                }
            }

            void TimeCheck()
            {
                if (waveCount >= 6)
                    return;

                waveInProgress = false;
                uint32 timeDifference = WAVE_TIME - waveTime;
                if (timeDifference <= TIME_DIFFERENCE)
                    events.RescheduleEvent(EVENT_WAVE, timeDifference);
                else
                    events.RescheduleEvent(EVENT_WAVE, TIME_DIFFERENCE);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetUlduarAI<boss_freyaAI>(creature);
        }
};

class boss_elder_brightleaf : public CreatureScript
{
    public:
        boss_elder_brightleaf() : CreatureScript("boss_elder_brightleaf") { }

        struct boss_elder_brightleafAI : public BossAI
        {
            boss_elder_brightleafAI(Creature* creature) : BossAI(creature, BOSS_BRIGHTLEAF)
            {
            }

            uint32 lumberjackTimer;
            uint8 elderCount;
            bool lumberjack;

            void Reset()
            {
                _Reset();
                if (me->HasAura(SPELL_DRAINED_OF_POWER))
                    me->RemoveAurasDueToSpell(SPELL_DRAINED_OF_POWER);
                events.ScheduleEvent(EVENT_SOLAR_FLARE, urand(5000, 7000));
                events.ScheduleEvent(EVENT_UNSTABLE_SUN_BEAM, urand(7000, 12000));
                events.ScheduleEvent(EVENT_FLUX, 5000);
                elderCount = 0;
                lumberjack = false;
            }

            void KilledUnit(Unit* who)
            {
                DoScriptText(RAND(SAY_BRIGHTLEAF_SLAY_1, SAY_BRIGHTLEAF_SLAY_2), me, who);
            }

            void JustDied(Unit* who)
            {
                _JustDied();
                DoScriptText(SAY_BRIGHTLEAF_DEATH, me, who);

                if (who && who->GetTypeId() == TYPEID_PLAYER)
                {
                    if (Creature* Ironbranch = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(BOSS_IRONBRANCH) : 0))
                        Ironbranch->AI()->DoAction(ACTION_ELDER_DEATH);

                    if (Creature* Stonebark = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(BOSS_STONEBARK) : 0))
                        Stonebark->AI()->DoAction(ACTION_ELDER_DEATH);
                }
            }

            void EnterCombat(Unit* who)
            {
                _EnterCombat();
                if (!me->HasAura(SPELL_DRAINED_OF_POWER))
                    DoScriptText(SAY_BRIGHTLEAF_AGGRO, me, who);
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim() || me->HasAura(SPELL_DRAINED_OF_POWER))
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STAT_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_UNSTABLE_SUN_BEAM:
                            Position pos;
                            me->GetRandomNearPosition(pos, 20.0f);
                            me->CastSpell(me, SPELL_UNSTABLE_SUN_BEAM_SUMMON, true);
                            events.ScheduleEvent(EVENT_UNSTABLE_SUN_BEAM, urand(10000, 15000));
                            break;
                        case EVENT_SOLAR_FLARE:
                        {
                            uint8 stackAmount = 0;
                            if (me->GetAura(SPELL_FLUX_AURA))
                                stackAmount = me->GetAura(SPELL_FLUX_AURA)->GetStackAmount();
                            me->CastCustomSpell(SPELL_SOLAR_FLARE, SPELLVALUE_MAX_TARGETS, stackAmount, me, false);
                            events.ScheduleEvent(EVENT_SOLAR_FLARE, urand(5000, 10000));
                            break;
                        }
                        case EVENT_FLUX:
                            me->RemoveAurasDueToSpell(SPELL_FLUX_AURA);
                            me->AddAura(SPELL_FLUX_AURA, me);
                            if (Aura* Flux = me->GetAura(SPELL_FLUX_AURA))
                                Flux->SetStackAmount(urand(1, 8));
                            events.ScheduleEvent(EVENT_FLUX, 7500);
                            break;
                    }
                }

                if (lumberjack)
                    lumberjackTimer += diff;

                DoMeleeAttackIfReady();
            }

            void DoAction(int32 const action)
            {
                switch (action)
                {
                    case ACTION_ELDER_DEATH:
                        ++elderCount;
                        lumberjack = true;
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetUlduarAI<boss_elder_brightleafAI>(creature);
        }
};

class boss_elder_stonebark : public CreatureScript
{
    public:
        boss_elder_stonebark() : CreatureScript("boss_elder_stonebark") { }

        struct boss_elder_stonebarkAI : public BossAI
        {
            boss_elder_stonebarkAI(Creature* creature) : BossAI(creature, BOSS_STONEBARK)
            {
            }

            uint32 lumberjackTimer;
            uint8 elderCount;
            bool lumberjack;

            void Reset()
            {
                _Reset();
                if (me->HasAura(SPELL_DRAINED_OF_POWER))
                    me->RemoveAurasDueToSpell(SPELL_DRAINED_OF_POWER);
                events.ScheduleEvent(EVENT_TREMOR, urand(10000, 12000));
                events.ScheduleEvent(EVENT_FISTS, urand(25000, 35000));
                events.ScheduleEvent(EVENT_BARK, urand(37500, 40000));
                elderCount = 0;
                lumberjack = false;
            }

            void KilledUnit(Unit* who)
            {
                DoScriptText(RAND(SAY_STONEBARK_SLAY_1, SAY_STONEBARK_SLAY_2), me, who);
            }

            void JustDied(Unit* who)
            {
                _JustDied();
                DoScriptText(SAY_STONEBARK_DEATH, me, who);

                if (who && who->GetTypeId() == TYPEID_PLAYER)
                {
                    if (Creature* Ironbranch = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(BOSS_IRONBRANCH) : 0))
                        Ironbranch->AI()->DoAction(ACTION_ELDER_DEATH);

                    if (Creature* Brightleaf = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(BOSS_BRIGHTLEAF) : 0))
                        Brightleaf->AI()->DoAction(ACTION_ELDER_DEATH);
                }
            }

            void EnterCombat(Unit* who)
            {
                _EnterCombat();
                if (!me->HasAura(SPELL_DRAINED_OF_POWER))
                    DoScriptText(SAY_STONEBARK_AGGRO, me, who);
            }

            void DamageTaken(Unit* who, uint32& damage)
            {
                if (who == me)
                    return;

                if (me->HasAura(SPELL_PETRIFIED_BARK))
                {
                    int32 reflect = damage;
                    who->CastCustomSpell(who, SPELL_PETRIFIED_BARK_DMG, &reflect, NULL, NULL, true);
                    damage = 0;
                }
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim() || me->HasAura(SPELL_DRAINED_OF_POWER))
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STAT_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BARK:
                            DoCast(me, SPELL_PETRIFIED_BARK);
                            events.ScheduleEvent(EVENT_BARK, urand(30000, 50000));
                            break;
                        case EVENT_FISTS:
                            DoCast(me->getVictim(), SPELL_FISTS_OF_STONE);
                            events.ScheduleEvent(EVENT_FISTS, urand(20000, 30000));
                            break;
                        case EVENT_TREMOR:
                            if (!me->HasAura(SPELL_FISTS_OF_STONE))
                                DoCast(me->getVictim(), SPELL_GROUND_TREMOR);
                            events.ScheduleEvent(EVENT_TREMOR, urand(10000, 20000));
                            break;
                    }
                }

                if (lumberjack)
                    lumberjackTimer += diff;

                DoMeleeAttackIfReady();
            }

            void DoAction(int32 const action)
            {
                switch (action)
                {
                    case ACTION_ELDER_DEATH:
                        ++elderCount;
                        lumberjack = true;
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetUlduarAI<boss_elder_stonebarkAI>(creature);
        }
};

class boss_elder_ironbranch : public CreatureScript
{
    public:
        boss_elder_ironbranch() : CreatureScript("boss_elder_ironbranch") { }

        struct boss_elder_ironbranchAI : public BossAI
        {
            boss_elder_ironbranchAI(Creature* creature) : BossAI(creature, BOSS_IRONBRANCH)
            {
            }

            uint32 lumberjackTimer;
            uint8 elderCount;
            bool lumberjack;

            void Reset()
            {
                _Reset();
                if (me->HasAura(SPELL_DRAINED_OF_POWER))
                    me->RemoveAurasDueToSpell(SPELL_DRAINED_OF_POWER);
                events.ScheduleEvent(EVENT_IMPALE, urand(18000, 22000));
                events.ScheduleEvent(EVENT_IRON_ROOTS, urand(12000, 17000));
                events.ScheduleEvent(EVENT_THORN_SWARM, urand(7500, 12500));
                elderCount = 0;
                lumberjack = false;
            }

            void KilledUnit(Unit* who)
            {
                DoScriptText(RAND(SAY_IRONBRANCH_SLAY_1, SAY_IRONBRANCH_SLAY_2), me, who);
            }

            void JustDied(Unit* who)
            {
                _JustDied();
                DoScriptText(SAY_IRONBRANCH_DEATH, me, who);

                if (who && who->GetTypeId() == TYPEID_PLAYER)
                {
                    if (Creature* Brightleaf = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(BOSS_BRIGHTLEAF) : 0))
                        Brightleaf->AI()->DoAction(ACTION_ELDER_DEATH);

                    if (Creature* Stonebark = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(BOSS_STONEBARK) : 0))
                        Stonebark->AI()->DoAction(ACTION_ELDER_DEATH);
                }
            }

            void EnterCombat(Unit* who)
            {
                _EnterCombat();
                if (!me->HasAura(SPELL_DRAINED_OF_POWER))
                    DoScriptText(SAY_IRONBRANCH_AGGRO, me, who);
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim() || me->HasAura(SPELL_DRAINED_OF_POWER))
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STAT_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_IMPALE:
                            DoCast(me->getVictim(), SPELL_IMPALE);
                            events.ScheduleEvent(EVENT_IMPALE, urand(15000, 25000));
                            break;
                        case EVENT_IRON_ROOTS:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true, -SPELL_ROOTS_IRONBRANCH))
                                target->CastSpell(target, SPELL_ROOTS_IRONBRANCH, true);
                            events.ScheduleEvent(EVENT_IRON_ROOTS, urand(10000, 20000));
                            break;
                        case EVENT_THORN_SWARM:
                            DoCast(me->getVictim(), SPELL_THORN_SWARM);
                            events.ScheduleEvent(EVENT_THORN_SWARM, urand(8000, 13000));
                            break;
                    }
                }

                if (lumberjack)
                    lumberjackTimer += diff;

                DoMeleeAttackIfReady();
            }

            void DoAction(int32 const action)
            {
                switch (action)
                {
                    case ACTION_ELDER_DEATH:
                        ++elderCount;
                        lumberjack = true;
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetUlduarAI<boss_elder_ironbranchAI>(creature);
        }
};

class npc_detonating_lasher : public CreatureScript
{
    public:
        npc_detonating_lasher() : CreatureScript("npc_detonating_lasher") { }

        struct npc_detonating_lasherAI : public ScriptedAI
        {
            npc_detonating_lasherAI(Creature* creature) : ScriptedAI(creature)
            {
                me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
            }

            uint32 lashTimer;
            uint32 changeTargetTimer;

            void Reset()
            {
                lashTimer = 5000;
                changeTargetTimer = 7500;
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                if (lashTimer <= diff)
                {
                    DoCast(SPELL_FLAME_LASH);
                    lashTimer = urand(5000, 10000);
                }
                else
                    lashTimer -= diff;

                if (changeTargetTimer <= diff)
                {
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                    {
                        // Switching to other target - modify aggro of new target by 20% from current target's aggro
                        me->AddThreat(target, me->getThreatManager().getThreat(me->getVictim(), false) * 1.2f);
                        me->AI()->AttackStart(target);
                    }
                    changeTargetTimer = urand(5000, 10000);
                }
                else
                    changeTargetTimer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_detonating_lasherAI(creature);
        }
};

class npc_ancient_water_spirit : public CreatureScript
{
    public:
        npc_ancient_water_spirit() : CreatureScript("npc_ancient_water_spirit") { }

        struct npc_ancient_water_spiritAI : public ScriptedAI
        {
            npc_ancient_water_spiritAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = me->GetInstanceScript();
                if (Creature* Freya = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(BOSS_FREYA) : 0))
                    waveCount = CAST_AI(boss_freya::boss_freyaAI, Freya->AI())->trioWaveCount;
            }

            InstanceScript* instance;

            uint32 tidalWaveTimer;
            uint8 waveCount;

            void Reset()
            {
                tidalWaveTimer = 10000;
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                if (tidalWaveTimer <= diff)
                {
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                    {
                        DoCast(target, SPELL_TIDAL_WAVE);
                        DoCast(target, SPELL_TIDAL_WAVE_EFFECT, true);
                    }
                    tidalWaveTimer = urand(12000, 25000);
                }
                else
                    tidalWaveTimer -= diff;

                DoMeleeAttackIfReady();
            }

            void JustDied(Unit* /*who*/)
            {
                if (Creature* Freya = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(BOSS_FREYA) : 0))
                {
                    CAST_AI(boss_freya::boss_freyaAI, Freya->AI())->checkElementalAlive[waveCount] = false;
                    CAST_AI(boss_freya::boss_freyaAI, Freya->AI())->LasherDead(1);
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_ancient_water_spiritAI(creature);
        }
};

class npc_storm_lasher : public CreatureScript
{
    public:
        npc_storm_lasher() : CreatureScript("npc_storm_lasher") { }

        struct npc_storm_lasherAI : public ScriptedAI
        {
            npc_storm_lasherAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = me->GetInstanceScript();
                if (Creature* Freya = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(BOSS_FREYA) : 0))
                    waveCount = CAST_AI(boss_freya::boss_freyaAI, Freya->AI())->trioWaveCount;
            }

            InstanceScript* instance;

            uint32 lightningLashTimer;
            uint32 stormboltTimer;
            uint8 waveCount;

            void Reset()
            {
                lightningLashTimer = 10000;
                stormboltTimer = 5000;
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                if (lightningLashTimer <= diff)
                {
                    DoCast(SPELL_LIGHTNING_LASH);
                    lightningLashTimer = urand(7000, 14000);
                }
                else
                    lightningLashTimer -= diff;

                if (stormboltTimer <= diff)
                {
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                        DoCast(target, SPELL_STORMBOLT);
                    stormboltTimer = urand(8000, 12000);
                }
                else
                    stormboltTimer -= diff;

                DoMeleeAttackIfReady();
            }

            void JustDied(Unit* /*who*/)
            {
                if (Creature* Freya = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(BOSS_FREYA) : 0))
                {
                    CAST_AI(boss_freya::boss_freyaAI, Freya->AI())->checkElementalAlive[waveCount] = false;
                    CAST_AI(boss_freya::boss_freyaAI, Freya->AI())->LasherDead(2);
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_storm_lasherAI(creature);
        }
};

class npc_snaplasher : public CreatureScript
{
    public:
        npc_snaplasher() : CreatureScript("npc_snaplasher") { }

        struct npc_snaplasherAI : public ScriptedAI
        {
            npc_snaplasherAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = me->GetInstanceScript();
                if (Creature* Freya = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(BOSS_FREYA) : 0))
                    waveCount = CAST_AI(boss_freya::boss_freyaAI, Freya->AI())->trioWaveCount;
            }

            InstanceScript* instance;

            uint8 waveCount;

            void UpdateAI(uint32 const /*diff*/)
            {
                if (!UpdateVictim())
                    return;

                if (!me->HasAura(SPELL_BARK_AURA))
                    DoCast(SPELL_HARDENED_BARK);

                DoMeleeAttackIfReady();
            }

            void JustDied(Unit* /*who*/)
            {
                if (Creature* Freya = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(BOSS_FREYA) : 0))
                {
                    CAST_AI(boss_freya::boss_freyaAI, Freya->AI())->checkElementalAlive[waveCount] = false;
                    CAST_AI(boss_freya::boss_freyaAI, Freya->AI())->LasherDead(4);
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_snaplasherAI(creature);
        }
};

class npc_ancient_conservator : public CreatureScript
{
    public:
        npc_ancient_conservator() : CreatureScript("npc_ancient_conservator") { }

        struct npc_ancient_conservatorAI : public ScriptedAI
        {
            npc_ancient_conservatorAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            uint32 natureFuryTimer;
            uint32 healthySporeTimer;
            bool grip;

            void Reset()
            {
                natureFuryTimer = 7500;
                healthySporeTimer = 3500;
                SummonHealthySpores(2);
                grip = false;
            }

            void SummonHealthySpores(uint8 sporesCount)
            {
                for (uint8 n = 0; n < sporesCount; ++n)
                {
                    DoCast(SPELL_SUMMON_PERIODIC);
                    DoCast(SPELL_SPORE_SUMMON_NE);
                    DoCast(SPELL_SPORE_SUMMON_SE);
                    DoCast(SPELL_SPORE_SUMMON_SW);
                }
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                if (!grip)
                {
                    me->CastSpell(me->getVictim(), SPELL_CONSERVATOR_GRIP, true);
                    grip = true;
                }

                if (healthySporeTimer <= diff)
                {
                    SummonHealthySpores(1);
                    healthySporeTimer = urand(15000, 17500);
                }
                else
                    healthySporeTimer -= diff;

                if (natureFuryTimer <= diff)
                {
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true, -SPELL_NATURE_FURY))
                        DoCast(target, SPELL_NATURE_FURY);
                    me->AddAura(SPELL_CONSERVATOR_GRIP, me);
                    natureFuryTimer = 5000;
                }
                else
                    natureFuryTimer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_ancient_conservatorAI(creature);
        }
};

class npc_sun_beam : public CreatureScript
{
    public:
        npc_sun_beam() : CreatureScript("npc_sun_beam") { }

        struct npc_sun_beamAI : public Scripted_NoMovementAI
        {
            npc_sun_beamAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);
                DoCastAOE(SPELL_FREYA_UNSTABLE_ENERGY_VISUAL, true);
                DoCast(SPELL_FREYA_UNSTABLE_ENERGY);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_sun_beamAI(creature);
        }
};

class npc_healthy_spore : public CreatureScript
{
    public:
        npc_healthy_spore() : CreatureScript("npc_healthy_spore") { }

        struct npc_healthy_sporeAI : public Scripted_NoMovementAI
        {
            npc_healthy_sporeAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_OOC_NOT_ATTACKABLE);
                me->SetReactState(REACT_PASSIVE);
                DoCast(me, SPELL_HEALTHY_SPORE_VISUAL);
                DoCast(me, SPELL_POTENT_PHEROMONES);
                DoCast(me, SPELL_GROW);
                lifeTimer = urand(22000, 30000);
            }

            uint32 lifeTimer;

            void UpdateAI(uint32 const diff)
            {
                if (lifeTimer <= diff)
                {
                    me->RemoveAurasDueToSpell(SPELL_GROW);
                    me->ForcedDespawn(2200);
                    lifeTimer = urand(22000, 30000);
                }
                else
                    lifeTimer -= diff;
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_healthy_sporeAI(creature);
        }
};

class npc_eonars_gift : public CreatureScript
{
    public:
        npc_eonars_gift() : CreatureScript("npc_eonars_gift") { }

        struct npc_eonars_giftAI : public Scripted_NoMovementAI
        {
            npc_eonars_giftAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                lifeBindersGiftTimer = 12000;
                DoCast(me, SPELL_GROW);
                DoCast(me, SPELL_PHEROMONES, true);
                DoCast(me, SPELL_EONAR_VISUAL, true);
            }

            uint32 lifeBindersGiftTimer;

            void UpdateAI(uint32 const diff)
            {
                if (lifeBindersGiftTimer <= diff)
                {
                    me->RemoveAurasDueToSpell(SPELL_GROW);
                    DoCast(SPELL_LIFEBINDERS_GIFT);
                    me->ForcedDespawn(2500);
                    lifeBindersGiftTimer = 12000;
                }
                else
                    lifeBindersGiftTimer -= diff;
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_eonars_giftAI(creature);
        }
};

class npc_nature_bomb : public CreatureScript
{
    public:
        npc_nature_bomb() : CreatureScript("npc_nature_bomb") { }

        struct npc_nature_bombAI : public Scripted_NoMovementAI
        {
            npc_nature_bombAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                bombTimer = urand(8000, 10000);
                DoCast(SPELL_OBJECT_BOMB);
            }

            uint32 bombTimer;

            void UpdateAI(uint32 const diff)
            {
                if (bombTimer <= diff)
                {
                    if (GameObject* go = me->FindNearestGameObject(OBJECT_NATURE_BOMB, 1.0f))
                    {
                        DoCast(me, SPELL_NATURE_BOMB);
                        me->RemoveGameObject(go, true);
                        me->RemoveFromWorld();
                    }

                    bombTimer = 10000;
                }
                else
                    bombTimer -= diff;
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_nature_bombAI(creature);
        }
};

class npc_unstable_sun_beam : public CreatureScript
{
    public:
        npc_unstable_sun_beam() : CreatureScript("npc_unstable_sun_beam") { }

        struct npc_unstable_sun_beamAI : public Scripted_NoMovementAI
        {
            npc_unstable_sun_beamAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                despawnTimer = urand(7000, 12000);
                instance = me->GetInstanceScript();
                DoCast(me, SPELL_PHOTOSYNTHESIS);
                DoCast(me, SPELL_UNSTABLE_SUN_BEAM);
                me->SetReactState(REACT_PASSIVE);
            }

            InstanceScript* instance;

            uint32 despawnTimer;

            void UpdateAI(uint32 const diff)
            {
                if (despawnTimer <= diff)
                {
                    DoCastAOE(SPELL_UNSTABLE_ENERGY, true);
                    me->DisappearAndDie();
                }
                else
                    despawnTimer -= diff;
            }

            void SpellHitTarget(Unit* target, SpellEntry const* spell)
            {
                if (target && spell->Id == SPELL_UNSTABLE_ENERGY)
                {
                    target->RemoveAurasDueToSpell(SPELL_UNSTABLE_SUN_BEAM);
                    target->RemoveAurasDueToSpell(SPELL_UNSTABLE_SUN_BEAM_TRIGGERED);
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_unstable_sun_beamAI(creature);
        }
};

class spell_attuned_to_nature_dose_reduction : public SpellScriptLoader
{
public:
    spell_attuned_to_nature_dose_reduction() : SpellScriptLoader("spell_attuned_to_nature_dose_reduction")
    {
    }

    class spell_attuned_to_nature_dose_reduction_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_attuned_to_nature_dose_reduction_SpellScript)

        void HandleScript(SpellEffIndex /*effIndex*/)
        {
            Unit* target = GetHitUnit();
            SpellEntry const* spellInfo = GetSpellInfo();
            switch (spellInfo->Id)
            {
                case SPELL_ATTUNED_TO_NATURE_2_DOSE_REDUCTION:
                    if (target->HasAura(GetEffectValue()))
                        for (uint8 n = 0; n < 2; ++n)
                            target->RemoveAuraFromStack(GetEffectValue(), 0, AURA_REMOVE_BY_DEFAULT);
                    break;
                case SPELL_ATTUNED_TO_NATURE_10_DOSE_REDUCTION:
                    if (target->HasAura(GetEffectValue()))
                        for (uint8 n = 0; n < 10; ++n)
                            target->RemoveAuraFromStack(GetEffectValue(), 0, AURA_REMOVE_BY_DEFAULT);
                    break;
                case SPELL_ATTUNED_TO_NATURE_25_DOSE_REDUCTION:
                    if (target->HasAura(GetEffectValue()))
                        for (uint8 n = 0; n < 25; ++n)
                            target->RemoveAuraFromStack(GetEffectValue(), 0, AURA_REMOVE_BY_DEFAULT);
                    break;
                default:
                    break;
            }
        }

        void Register()
        {
            OnEffect += SpellEffectFn(spell_attuned_to_nature_dose_reduction_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_attuned_to_nature_dose_reduction_SpellScript();
    }
};

class spell_freya_iron_roots : public SpellScriptLoader
{
    public:
        spell_freya_iron_roots() : SpellScriptLoader("spell_freya_iron_roots")
        {
        }

        class spell_freya_iron_roots_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_freya_iron_roots_SpellScript);

            void HandleSummon(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                uint32 entry = uint32(GetSpellInfo()->EffectMiscValue[effIndex]);

                Position pos;
                GetCaster()->GetPosition(&pos);
                // Not good at all, but this prevents having roots in a different position then player
                if (Creature* Roots = GetCaster()->SummonCreature(entry, pos))
                    GetCaster()->NearTeleportTo(Roots->GetPositionX(), Roots->GetPositionY(), Roots->GetPositionZ(), GetCaster()->GetOrientation());
            }

            void Register()
            {
                OnEffect += SpellEffectFn(spell_freya_iron_roots_SpellScript::HandleSummon, EFFECT_0, SPELL_EFFECT_SUMMON);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_freya_iron_roots_SpellScript();
        }
};

void AddSC_boss_freya()
{
    new boss_freya();
    new boss_elder_brightleaf();
    new boss_elder_ironbranch();
    new boss_elder_stonebark();
    new npc_ancient_conservator();
    new npc_snaplasher();
    new npc_storm_lasher();
    new npc_ancient_water_spirit();
    new npc_detonating_lasher();
    new npc_sun_beam();
    new npc_nature_bomb();
    new npc_eonars_gift();
    new npc_healthy_spore();
    new npc_unstable_sun_beam();
    new npc_iron_roots();
    new spell_attuned_to_nature_dose_reduction();
    new spell_freya_iron_roots();
}
