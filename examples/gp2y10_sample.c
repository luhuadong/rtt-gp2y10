


/* cat_gp2y10 */
static void cat_gp2y10(void)
{
    struct gp2y10_device gp2y10;

    gp2y10_init(&gp2y10, GP2Y10_ILED_PIN, GP2Y10_AOUT_PIN);
    float dust = gp2y10_get_dust_density(&gp2y10);
    rt_kprintf("(GP2Y10) Dust: %d.%02d ug/m3\n", (int)dust, (int)(dust*100)%100);
}
#ifdef FINSH_USING_MSH
MSH_CMD_EXPORT(cat_gp2y10, read gp2y10 dust density);
#endif