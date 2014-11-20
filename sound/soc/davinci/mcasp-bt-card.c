/*
 * mcasp-bt-card.c  --  SoC audio for McASP-based Bluetooth SCO
 *
 * Author: Misael Lopez Cruz <misael.lopez@ti.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/of_platform.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/soc.h>

struct bt_card_data {
	unsigned int bclk_rate;
};

static int mcasp_bt_startup(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;

	snd_pcm_hw_constraint_minmax(runtime, SNDRV_PCM_HW_PARAM_RATE,
				     8000, 8000);
	snd_pcm_hw_constraint_minmax(runtime, SNDRV_PCM_HW_PARAM_CHANNELS,
				     2, 2);
	snd_pcm_hw_constraint_mask64(runtime, SNDRV_PCM_HW_PARAM_FORMAT,
				     SNDRV_PCM_FMTBIT_S16_LE);

	return 0;
}

static int mcasp_bt_hw_params(struct snd_pcm_substream *substream,
			      struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_card *card = rtd->card;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	struct bt_card_data *card_data = snd_soc_card_get_drvdata(card);
	int min_bclk = snd_soc_params_to_bclk(params);
	int ret;

	ret = snd_soc_dai_set_sysclk(cpu_dai, 0, 0, SND_SOC_CLOCK_OUT);
	if (ret < 0) {
		dev_err(card->dev, "can't set CPU DAI sysclk %d\n", ret);
		return ret;
	}

	if (card_data->bclk_rate > min_bclk) {
		/*
		 * Bluetooth SCO is 8kHz, mono, 16-bits/sample but the BCLK
		 * may be running at a higher rate. The BCLK / FSYNC ratio
		 * must be explicitly set for those cases.
		 */
		ret = snd_soc_dai_set_clkdiv(cpu_dai, 2,
				card_data->bclk_rate / params_rate(params));
		if (ret < 0)
			dev_err(card->dev,
				"can't set CPU DAI BCLK/FSYNC ratio %d\n", ret);
	}

	return ret;
}

static struct snd_soc_ops mcasp_bt_ops = {
	.startup = mcasp_bt_startup,
	.hw_params = mcasp_bt_hw_params,
};

static struct snd_soc_dai_link dra7_evm_wl8_link = {
	.name		= "DRA7xx WiLink",
	.stream_name	= "Bluetooth SCO",
	.codec_name	= "snd-soc-dummy",
	.codec_dai_name	= "snd-soc-dummy-dai",
	.platform_name	= "omap-pcm-audio",
	.ops		= &mcasp_bt_ops,
	.dai_fmt	= (SND_SOC_DAIFMT_DSP_A |
			   SND_SOC_DAIFMT_NB_IF |
			   SND_SOC_DAIFMT_CBM_CFM),
};

static const struct of_device_id mcasp_bt_of_ids[] = {
	{
		.compatible = "ti,dra7xx-wl8-bt",
		.data = &dra7_evm_wl8_link,
	},
	{ /* sentinel */ },
};
MODULE_DEVICE_TABLE(of, mcasp_bt_of_ids);


/* Audio machine driver */
static struct snd_soc_card mcasp_bt_card = {
	.owner = THIS_MODULE,
	.num_links = 1,
};

static int mcasp_bt_snd_probe(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node;
	const struct of_device_id *match =
		of_match_device(of_match_ptr(mcasp_bt_of_ids), &pdev->dev);
	struct snd_soc_dai_link *dai = (struct snd_soc_dai_link *)match->data;
	struct snd_soc_card *card = &mcasp_bt_card;
	struct bt_card_data *card_data;
	int ret;

	if (!np) {
		dev_err(card->dev, "missing of_node\n");
		return -ENODEV;
	}

	card->dev = &pdev->dev;
	card->dai_link = dai;

	card_data = devm_kzalloc(&pdev->dev, sizeof(*card_data), GFP_KERNEL);
	if (!card_data)
		return -ENOMEM;

	ret = snd_soc_of_parse_card_name(card, "ti,model");
	if (ret) {
		dev_err(card->dev, "card name is not provided\n");
		return -ENODEV;
	}

	dai->cpu_of_node = of_parse_phandle(np, "ti,mcasp-controller", 0);
	if (!dai->cpu_of_node)
		return -EINVAL;

	/* Get the bit clock frequency (when supplied externally) */
	of_property_read_u32(np, "ti,bclk-rate", &card_data->bclk_rate);

	snd_soc_card_set_drvdata(card, card_data);

	ret = snd_soc_register_card(card);
	if (ret)
		dev_err(card->dev, "failed to register sound card %d\n", ret);

	return ret;
}

static int mcasp_bt_snd_remove(struct platform_device *pdev)
{
	struct snd_soc_card *card = platform_get_drvdata(pdev);

	snd_soc_unregister_card(card);

	return 0;
}

static struct platform_driver mcasp_bt_snd_driver = {
	.driver = {
		.name = "mcasp-bt-sound",
		.owner = THIS_MODULE,
		.pm = &snd_soc_pm_ops,
		.of_match_table = mcasp_bt_of_ids,
	},
	.probe = mcasp_bt_snd_probe,
	.remove = mcasp_bt_snd_remove,
};

module_platform_driver(mcasp_bt_snd_driver);

MODULE_AUTHOR("Misael Lopez Cruz <misael.lopez@ti.com>");
MODULE_DESCRIPTION("ALSA SoC for McASP-based Bluetooth cards");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:mcasp-bt-sound");
